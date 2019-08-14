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
#include "NGramGraph.hpp"
#include "DocumentClass.hpp"
#include "OclUpdatableClass.hpp"
#include "StringSplitter.hpp"
#include "StringPayload.hpp"
#include "GraphComparator.hpp"
#include "GraphSimilarity.hpp"
#include "ProximityApproach.hpp"

#define TABLE_SIZE_DEFAULT_VALUE 5000000
#define SERIAL_CLASS_GRAPHS_RELATIVE_DIR "precise_class_graphs/"
#define TOPICS_RELATIVE_DIR "topics/"
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


void display_usage_message(std::string topics_dir) {
	std::cout << std::endl;
	std::cout << "A simple program to test the efficacy of the in parallel constructed class graphs "
		  << "in comparison with the serially constructed ones.\nThe program accepts the following options/arguments:\n"
		  << "\t -h :                   Prints this message.
		  << "\t -c :                   Reports an execution time comparison between serial and parallel methods.\n"
		  << "\t                        It is *very* slow, as the class graphs of the serial method are computed from\n"
		  << "\t                        scratch (in order to be measured). If this option is not enabled, the graphs\n"
		  << "\t                        of the serial method are read from a file.\n"
		  << "\t -f <filename> :        (Mandatory) Each line of the <filename> file must contain two entries, separated\n"
		  << "\t                        by a whitespace. The first entry should be the filesystem path to a\n"
		  << "\t                        text sample and the second entry the text sample's topic. The program\n"
		  << "\t                        will try to extract the topic of each sample using both methods, serial,\n"
		  << "\t                        and parallel, and check if it was right.\n"
		  << "\t                        The given topics must be one of those in the '" << topics_dir << "' directory.\n" 
		  << "\t                        You can add more topics in that directory in order to be able to include them in the "
		  << "\t                        given topics.\n"
		  << "\t -s <hash_table_size> : The hash table size to be used in the parallel method. Default value is 5000000.\n";
	std::cout << std::endl;
}


int main(int argc, char **argv) {
	const std::string serial_class_graphs_dir(argv[0]);
	const std::string topics_dir(argv[0]);
	const std::string kernels_dir(argv[0]);
	if (serial_class_graphs_dir.back() != '/') {
		serial_class_graphs_dir += "/";
	}
	serial_class_graphs_dir += SERIAL_CLASS_GRAPHS_RELATIVE_DIR;
	if (topics_dir.back() != '/') {
		topics_dir += "/";
	}
	topics_dir += TOPICS_RELATIVE_DIR;
	if (kernels_dir.back() != '/') {
		kernels_dir += "/";
	}
	kernels_dir += OPENCL_KERNELS_RELATIVE_DIR;

	InputParser iParser(argc, argv);
	const std::string filename = iParser.getCmdOption("-f");
	const std::string number = iParser.getCmdOption("-s");
	const unsigned int table_size;
	if (filename.empty()) {
		diplay_usage_message(topics_dir);
		exit(0);
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
		cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu) << std::endl;
		exit(1);
	}
	else {
		cout << "OpenCL program was built successfully.\n";
	}
	/* OpenCL initialized successfully */


	std::vector<std::strings> topics;
	FileUtils::read_directory(topics_dir, topics);

	std::ifstream iFile(filename);
	std::vector<std:pair<std::string, std::string>> input(0);
	std::string sample, topic;
	while (iFile >> sample >> topic) {
		if (find(topics.begin(), topics.end(), topic) != topics.end()) {
			input.push_back(std::make_pair(sample, topic));
		}
		else {
			std::cout << "No dataset for topic '" << topic "'. Sample '" << sample "' will be excluded." << std::endl;
		}
	}

	if (input.size() == 0) {
		std::cout << std::endl << "There is no dataset for any of the topics given." << std::endl;
		exit(0);
	}

	std::vector<NGramGraph *> sample_graphs(0);
	NGramGraph *sample_ngg;
	StringSplitter spliter;
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

	GraphComparator gComp;
	GraphSimilarity gSim;
	DocumentClass *docClass_serial;
	OclUpdatableClass *docClass_parallel;

	int sample_cnt;
	double tempVS;
	for (auto& topic : topics) {
		docClass_serial = read_class_graph_from_file(serial_class_graphs_dir + topic + ".cg");
		docClass_parallel = new OclUpdatableClass(table_size);
		docClass_parallel->buildClass(topics_dir + topic + "/", &context, &queue, &program);

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

	std::cout << std::endl << "Sample file\t\tSerial extraction\t\tOpenCL extraction" << std::endl;
	sample_cnt = 0;
	for (auto& line : input) {
		std::cout << line.first;
		if (line.first.length() < 8) std::cout << "\t\t\t";
		else if (line.first.length() < 16) std::cout << "\t\t";
		else std::cout << "\t";

		if (line.second == extractedTopics[sample_cnt].first) std::cout << "CORRECT\t\t\t";
		else std::cout << "WRONG\t\t\t";

		if (line.second == extractedTopics[sample_cnt].second) std::cout << "CORRECT\t\t\t";
		else std::cout << "WRONG\t\t\t";

		std::cout << std::endl;
		++sample_cnt;
	}

	return 0;
}
