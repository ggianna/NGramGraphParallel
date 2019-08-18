#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#ifdef __APPLE_
   #include <OpenCL/cl.hpp>
#else
   #include <CL/cl.hpp>
#endif

#include "InputParser.hpp"
#include "Atom.hpp"
#include "NGramGraph.hpp"
#include "DocumentClass.hpp"
#include "OclUpdatableClass.hpp"
#include "StringSplitter.hpp"
#include "StringPayload.hpp"
#include "GraphComparator.hpp"
#include "GraphSimilarity.hpp"
#include "ProximityApproach.hpp"

#define TABLE_SIZE_DEFAULT_VALUE 5000000
#define SERIAL_CLASS_GRAPHS_RELATIVE_DIR "class_graphs_serial/"
#define PARALLEL_CLASS_GRAPHS_RELATIVE_DIR "class_graphs_parallel/"
#define TOPICS_RELATIVE_DIR "topics"
#define OPENCL_KERNELS_RELATIVE_DIR "ngg_opencl_kernels/"


/* 
 * Returns the first platform found.
 */
Platform getPlatform() {
    std::vector<Platform> all_platforms;
    Platform::get(&all_platforms);

    if (all_platforms.size()==0) {
        cout << "No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    return all_platforms[0];
}


/*
 * Returns the deviced specified by the index i on platform.
 * If display is true, then all of the platforms are listed.
 */
Device getDevice(Platform platform, cl_device_type type, int i, bool display=false) {
    std::vector<Device> all_gpus;
    platform.getDevices(type, &all_gpus);
    if (all_gpus.size() == 0) {
        cout << "No devices found. Check OpenCL installation!\n";
        exit(1);
    }

    if (display) {
        for (unsigned int j=0; j<all_gpus.size(); j++)
            printf("Device %d: %s\n", j, all_gpus[j].getInfo<CL_DEVICE_NAME>().c_str());
    }
    return all_gpus[i];
}


double compute_elapsed_time(struct timespec *start, struct timespec *finish) {
	double result = finish->tv_sec - start->tv_sec;
	result += (finish->tv_nsec - start->tv_nsec) / 1000000000.0;

	return result;
}


std::pair<DocumentClass *, double> getSerialClass(std::string topics_dir, std::string graphs_dir,
		std::string topic, bool count_time)
{
	struct timespec start, finish;
	double elapsed_time = -1.0;
	DocumentClass *class_graph = new DocumentClass();
	if (graphs_dir.back() != '/') {
		graphs_dir += "/";
	}
	if (topics_dir.back() != '/') {
		topics_dir += "/";
	}
	bool cached = FileUtils::fileExists(graphs_dir + topic + ".cg");
	if (count_time || !cached) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		class_graph->build(topics_dir + topic + "/");
		clock_gettime(CLOCK_MONOTONIC, &finish);
		elapsed_time = compute_elapsed_time(&start, &finish);
		if (!cached) {
			class_graph->cache(graphs_dir, topic + ".cg");
		}
	}
	else {
		class_graph->restoreFromFile(graphs_dir + topic + ".cg");
	}

	return {class_graph, elapsed_time};
}



std::pair<OclUpdatableClass *, double> getParallelClass(std::string topics_dir, std::string graphs_dir,
		std::string topic, unsigned int table_size, bool count_time, Context *c, CommandQueue *q, Program *p)
{
	struct timespec start, finish;
	OclUpdatableClass *class_graph = new OclUpdatableClass(table_size);
	double elapsed_time = -1.0;
	if (graphs_dir.back() != '/') {
		graphs_dir += "/";
	}
	if (topics_dir.back() != '/') {
		topics_dir += "/";
	}
	bool cached = FileUtils::fileExists(graphs_dir + std::to_string(table_size) + "/" + topic + ".cg");
	if (count_time || !cached) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		class_graph->buildClass(topics_dir + topic + "/", c, q, p);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		elapsed_time = compute_elapsed_time(&start, &finish);
		if (!cached) {
			class_graph->cache(graphs_dir + std::to_string(table_size) + "/", topic + ".cg");
		}
	}
	else {
		class_graph->restoreFromFile(graphs_dir + std::to_string(table_size) + "/" + topic + ".cg");
	}

	return {class_graph, elapsed_time};
}



void display_usage_message(std::string topics_dir, std::string serial_dir, std::string parallel_dir) {
	//std::cout << std::endl;
	std::cout << "A simple program to test the efficacy of the in parallel constructed class graphs "
		  << "in comparison with the ones constructed serially.\n\nThe program accepts the following options/arguments:\n"
		  << "\t -h                            Prints this message.\n"
		  << "\t -c [serial | serial-parallel] Uses only topics that have their class graphs cached to a file, so they don't\n"
		  << "\t                               need to be computed during program execution. It can be applied for the serial\n"
		  << "\t                               method only or for both parallel and serial methods. Default is for serial only.\n"
		  << "\t                               The available cached class graphs can be seen in the topics '" << serial_dir << "' and \n"
		  << "\t                               '" << parallel_dir << "' for the serial and parallel method respectively.\n"
		  << "\t -t                            Reports an execution time comparison between serial and parallel methods.\n"
		  << "\t                               It is *very* slow, as the class graphs of both  methods, serial and parallel, are\n"
		  << "\t                               computed from scratch (in order to be measured). If this option is not enabled, the graphs\n"
		  << "\t                               that happen to be cached to a file are read from there.\n"
		  << "\t -f <filename>                 (Mandatory) Each line of the <filename> file must contain two entries, separated\n"
		  << "\t                               by a whitespace. The first entry should be the absolute filesystem path to a\n"
		  << "\t                               text document and the second entry the document's topic. The program\n"
		  << "\t                               will try to extract the topic of each document using both methods, serial\n"
		  << "\t                               and parallel, and check if it was right.\n"
		  << "\t                               The given topics must be one of those in the '" << topics_dir << "' directory.\n" 
		  << "\t                               You can add more topics in that directory in order to be able to include them in the\n"
		  << "\t                               given topics.\n"
		  << "\t -s <hash_table_size>          The hash table size to be used in the parallel method. Default is 5000000.\n";
	std::cout << std::endl;
}


int main(int argc, char **argv) {
	std::string project_dir(argv[0]);
	project_dir = project_dir.substr(0, project_dir.find_last_of("/")+1);
	std::string serial_class_graphs_dir(project_dir + SERIAL_CLASS_GRAPHS_RELATIVE_DIR);
	std::string parallel_class_graphs_dir(project_dir + PARALLEL_CLASS_GRAPHS_RELATIVE_DIR);
	std::string topics_dir(project_dir + TOPICS_RELATIVE_DIR);
	std::string kernels_dir(project_dir + OPENCL_KERNELS_RELATIVE_DIR);

	InputParser iParser(argc, argv);
	const std::string& filename = iParser.getCmdOption("-f");
	const std::string& number = iParser.getCmdOption("-s");
	unsigned int table_size;
	bool time_comparison = iParser.cmdOptionExists("-t");
	bool cached_enabled = iParser.cmdOptionExists("-c");
	const std::string& cached_option = iParser.getCmdOption("-c");
	if (!cached_option.empty() && cached_option != "serial" && cached_option != "serial-parallel") {
		std::cout << "Invalid argument for the -c option. The value of the -c parameter should be either 'serial' or 'serial-parallel'.\n";
		exit(1);
	}
	if (filename.empty()) {
		display_usage_message(topics_dir, serial_class_graphs_dir, parallel_class_graphs_dir);
		exit(1);
	}
	if (cached_enabled && time_comparison) {
		std::cout << "Options -c and -t can't be enabled at the same time.\n";
		exit(1);
	}
	if (number.empty()) {
		table_size = TABLE_SIZE_DEFAULT_VALUE;
	}
	else {
		std::istringstream iss(number);
		iss >> table_size;
	}


	/* OpenCL initialization */
	Platform platform = getPlatform();
	Device gpu = getDevice(platform, CL_DEVICE_TYPE_GPU, 0);
	Context context({gpu});
	CommandQueue queue(context, gpu);
	Program::Sources sources;

	std::vector<std::string> kernel_files;
	std::string kernel_code;
	FileUtils::read_directory(kernels_dir, kernel_files);
	for (auto& file : kernel_files) {
		std::cout << "Reading file " << file << std::endl;
		kernel_code = FileUtils::read_file_to_string(kernels_dir + file);
		sources.push_back({kernel_code.c_str(), kernel_code.length()});
	}

	Program program(context, sources);
	if (program.build({gpu}) != CL_SUCCESS) {
		std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu) << std::endl;
		exit(1);
	}
	else {
		std::cout << "OpenCL program was built successfully.\n\n";
	}
	/* OpenCL initialized successfully */


	std::vector<std::string> topics;
	FileUtils::read_directory(topics_dir, topics);
	std::vector<std::string> used_topics = topics;

	// If the cached option is enabled, filter the topics to the ones that have a cached class graph.
	if (cached_enabled) {
		for (auto& topic : topics) {
			if (cached_option == "serial-parallel") {
				if (!FileUtils::fileExists(serial_class_graphs_dir + topic + ".cg")) {
					std::cout << "Class graph of topic '" << topic << "' for the serial method is not cached."
						  << "Topic '" << topic << "' won't be used.\n";
					used_topics.erase(std::remove(used_topics.begin(), used_topics.end(), topic), used_topics.end());
				}
				else if (!FileUtils::fileExists(parallel_class_graphs_dir + std::to_string(table_size) + "/" + topic + ".cg")) {
					std::cout << "Class graph of topic '" << topic << "' for the parallel method with hash table size of "
						  << table_size << " is not cached. Topic '" << topic << "' won't be used.\n";
					used_topics.erase(std::remove(used_topics.begin(), used_topics.end(), topic), used_topics.end());
				}
			}
			else if (cached_option == "serial" || cached_option.empty()) {
				if (!FileUtils::fileExists(serial_class_graphs_dir + topic + ".cg")) {
					std::cout << "Class graph of topic '" << topic << "' for the serial method is not cached."
						  << "Topic '" << topic << "' won't be used.\n";
					used_topics.erase(std::remove(used_topics.begin(), used_topics.end(), topic), used_topics.end());
				}
			}
		}
	}

	std::cout << std::endl;

	std::ifstream iFile(filename);
	std::vector<std::pair<std::string, std::string>> input(0);
	std::string sample, topic;
	/*
	std::vector<std::string> accepted_topics(0), rejected_topics(0);
	bool accepted, rejected;
	char answer = 'y';
	*/
	while (iFile >> sample >> topic) {
		if (std::find(used_topics.begin(), used_topics.end(), topic) != used_topics.end()) { // If this topic is used
			input.push_back(std::make_pair(sample, topic));
			/*
			// For topics with no cached class graphs, we give the option to skip them, because constructing them is very slow.
			rejected = std::find(rejected_topics.begin(), rejected_topics.end(), topic) != rejected_topics.end();
			if (rejected) { // Cached class graphs don't exist for this topic and the user has already rejected it.
				continue;
			}
			accepted = std::find(accepted_topics.begin(), accepted_topics.end(), topic) != accepted_topics.end();
			if (!(FileUtils::fileExists(serial_class_graphs_dir + topic + ".cg")) && !accepted) { //No cached class graph and not accepted
				if (!(FileUtils::fileExists(parallel_class_graphs_dir + std::to_string(table_size) + "/" + topic + ".cg"))) {
					std::cout << "No cached class graphs of both serial and parallel method for topic '"
						  << topic << "'. Compute them now?(Y/n): ";
					std::cout.flush();
				}
				else {
					std::cout << "No cached class graph of serial method for topic '" << topic << "'."
						  << " Compute one now?(Y/n): ";
					std::cout.flush();
				}
				std::cin >> answer;
			}
			else if (!(FileUtils::fileExists(parallel_class_graphs_dir + std::to_string(table_size) + "/" + topic + ".cg")) && !accepted) {
				std::cout << "No cached class graph of parallel method for topic '" << topic << "'."
					  << " Compute one now?(Y/n): ";
				std::cout.flush();
				std::cin >> answer;
			}

			if (answer != 'n' && answer != 'N') {
				input.push_back(std::make_pair(sample, topic));
				accepted_topics.push_back(topic);
			}
			else {
				answer = 'y';
				std::cout << "\tYou answered no. All documents of topic '" << topic << "' will be excluded.\n";
				rejected_topics.push_back(topic);
			}
			*/
		}
		else {
			std::cout << "Topic '" << topic << "' is not available. Document '" << sample << "' will be excluded." << std::endl;
		}
	}

	if (input.size() == 0) {
		std::cout << std::endl << "The filtered input set is empty. Exiting." << std::endl;
		exit(0);
	}

	std::vector<NGramGraph *> sample_graphs(0);
	NGramGraph *sample_ngg;
	StringSplitter splitter;
	StringPayload *payload;
	ProximityApproach *approach = new SymmetricApproach();


	for (auto& line : input) {
		sample = line.first;
		// TODO Check if sample exists
		payload = new StringPayload(FileUtils::read_file_to_string(sample));
		sample_ngg = new NGramGraph(nullptr, &splitter, payload, 4, approach);
		sample_ngg->createGraph();
		sample_graphs.push_back(sample_ngg);
	}

	// Holds the current max value similarity for every sample graph (for serial and parallel methods)
	std::vector<std::pair<double, double>> valueSimilarities(input.size(), std::make_pair(-1.0, -1.0));
	// Holds the current extracted topic for every sample graph (for serial and parallel methods)
	std::vector<std::pair<std::string, std::string>> extractedTopics(input.size());

	GraphComparator<std::string, std::string> gComp;
	GraphSimilarity gSim;
	DocumentClass *docClass_serial;
	OclUpdatableClass *docClass_parallel;

	if (time_comparison) {
		std::cout << std::endl << "Topic\t\t\tSerial Time(secs)\tParallel Time(secs)\tSpeedup\t\tValue Similarity\t" << std::endl;
	}
	int sample_cnt;
	double tempVS, serial_elapsed_time, parallel_elapsed_time;
	for (auto& topic : used_topics) {
		if (time_comparison) {
			std::cout << topic;
			if (topic.length() < 8) std::cout << "\t\t\t";
			else if (topic.length() < 16) std::cout << "\t\t";
			else if (topic.length() < 24) std::cout << "\t";
			//else hope topic.length() < 24
			std::cout.flush();
		}
		// TODO Check if file for class graph exists. If not, create it.
		//docClass_serial = read_class_graph_from_file(serial_class_graphs_dir + topic + ".cg");
		std::tie(docClass_serial, serial_elapsed_time) = getSerialClass(topics_dir, serial_class_graphs_dir, topic, time_comparison);
		if (time_comparison) {
			std::cout << std::setprecision(1) << std::fixed << serial_elapsed_time << "\t\t\t";
			std::cout.flush();
		}
		std::tie(docClass_parallel, parallel_elapsed_time) = getParallelClass(topics_dir, parallel_class_graphs_dir,
				topic, table_size, time_comparison, &context, &queue, &program);
		if (time_comparison) {
			std::cout << std::setprecision(1) << std::fixed << parallel_elapsed_time << "\t\t\t";
			std::cout << std::setprecision(2) << std::fixed << serial_elapsed_time / parallel_elapsed_time << "\t\t";
			std::cout.flush();
			gSim = gComp.compare(*docClass_serial, *docClass_parallel);
			tempVS = (gSim.getSimilarityComponents())["valueSimilarity"];
			std::cout << std::setprecision(2) << std::fixed << tempVS << "\t\t\t" << std::endl;
		}

		//docClass_serial = new DocumentClass(serial_class_graphs_dir + topic + ".cg");
		//docClass_parallel = new OclUpdatableClass(table_size);
		//docClass_parallel->buildClass(topics_dir + topic + "/", &context, &queue, &program);

		sample_cnt = 0;
		for (auto sample_graph : sample_graphs) {
			gSim = gComp.compare(*docClass_serial, *sample_graph);
			tempVS = (gSim.getSimilarityComponents())["valueSimilarity"];
			if (tempVS > valueSimilarities[sample_cnt].first) {
				valueSimilarities[sample_cnt].first = tempVS;
				extractedTopics[sample_cnt].first = topic;
			}

			gSim = gComp.compare(*docClass_parallel, *sample_graph);
			tempVS = (gSim.getSimilarityComponents())["valueSimilarity"];
			if (tempVS > valueSimilarities[sample_cnt].second) {
				valueSimilarities[sample_cnt].second = tempVS;
				extractedTopics[sample_cnt].second = topic;
			}

			++sample_cnt;
		}

		delete docClass_serial;
		delete docClass_parallel;
	}

	for (auto sample_graph : sample_graphs) {
		delete sample_graph;
	}
	sample_graphs.clear();

	if (time_comparison) std::cout << std::endl;
	std::cout << std::endl << "Document\t\tSerial Extraction\tOpenCL Extraction\tGiven Topic" << std::endl;
	sample_cnt = 0;
	std::string s_topic, p_topic;
	for (auto& line : input) {
		sample = line.first.substr(line.first.find_last_of("/")+1);
		std::cout << sample;
		if (sample.length() < 8) std::cout << "\t\t\t";
		else if (sample.length() < 16) std::cout << "\t\t";
		else std::cout << "\t"; // In this we hope sample.length() < 24.

		s_topic = extractedTopics[sample_cnt].first;
		std::cout << s_topic;
		if (s_topic.length() < 8) std::cout << "\t\t\t";
		else if (s_topic.length() < 16) std::cout << "\t\t";
		else std::cout << "\t"; // In this we hope s_topic.length() < 24.

		p_topic = extractedTopics[sample_cnt].second;
		std::cout << p_topic;
		if (p_topic.length() < 8) std::cout << "\t\t\t";
		else if (p_topic.length() < 16) std::cout << "\t\t";
		else std::cout << "\t"; // In this we hope p_topic.length() < 24.

		std::cout << line.second << std::endl;

		/*
		if (line.second == s_topic) std::cout "Correct\t\t\t";
		else std::cout "Wrong";

		if (line.second == p_topic) std::cout "Correct\t\t\t";
		else std::cout "Wrong";

		if (line.second == extractedTopics[sample_cnt].first) std::cout << "CORRECT\t\t\t";
		else std::cout << "WRONG\t\t\t";

		if (line.second == extractedTopics[sample_cnt].second) std::cout << "CORRECT\t\t\t";
		else std::cout << "WRONG\t\t\t";
		*/

		++sample_cnt;
	}

	std::cout << std::endl;

	return 0;
}
