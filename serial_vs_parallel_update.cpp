#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <CL/cl.hpp>

#include "NGramGraph.hpp"
#include "GraphComparator.hpp"
#include "GraphSimilarity.hpp"
#include "ProximityApproach.hpp"
#include "DocumentClass.hpp"
#include "FileUtils.hpp"

#define OPENCL_KERNELS_DIR "./ngg_opencl_kernels/"
#define OPENCL_HASH_TABLE_SIZE 2000000


Platform getPlatform() {
    /* Returns the first platform found. */
    std::vector<Platform> all_platforms;
    Platform::get(&all_platforms);

    if (all_platforms.size()==0) {
        cout << "No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    return all_platforms[0];
}


Device getDevice(Platform platform, cl_device_type type, int i, bool display=false) {
    /* Returns the deviced specified by the index i on platform.
     * If display is true, then all of the platforms are listed.
     */
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


double compute_elapsed_time(struct timespec *finish, struct timespec *start) {
	double result = finish->tv_sec - start->tv_sec;
	result += (finish->tv_nsec - start->tv_nsec) / 1000000000.0;

	return result;
}


DocumentClass * construct_class_graph_serial(std::string dir, bool precise = false) {
	// TODO Don't delete but reset the sample_ngg object
	StringSplitter splitter;
	StringPayload payload;
	ProximityApproach *approach = new SymmetricApproach();
	NGramGraph *sample_ngg;
	DocumentClass *class_graph = new DocumentClass();
	std::vector<std::string> files;
	FileUtils::read_directory(dir, files);

	if (dir.back() != '/') {
		dir += "/";
	}

	for (const auto& file : files) {
		// Get the contents of the file into a string and set the spayload with it.
		payload.setPayload(FileUtils::read_file_to_string(dir + file));

		// Create a temporary NGramGraph object representing the current file
		sample_ngg = new NGramGraph(nullptr, &splitter, &payload, 4, approach);
		sample_ngg->createGraph();

		// Update the current DocumentClass object with the above NGramGraph object
		class_graph->update(sample_ngg, precise);

		delete sample_ngg;
	}

	delete approach;

	return class_graph;
}

DocumentClass * construct_class_graph_parallel(std::string dir, Context *context, CommandQueue *queue, Program *program) {
	DocumentClass *class_graph = new DocumentClass(OPENCL_HASH_TABLE_SIZE);
	if (dir.back() != '/') {
		dir += "/";
	}
	class_graph->constructWithOpenCL(dir, context, queue, program);

	return class_graph;
}


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <topics_dir>\n"
			<< "\t<topics_dir> should be a directory containing subdirectories that contain text files.\n"
			<< "\tEach subdirectory should represent a topic.\n";
		exit(1);
	}

	/* OpenCL initialization */
	Platform platform = getPlatform();
	Device gpu = getDevice(platform, CL_DEVICE_TYPE_GPU, 0);
	Context context({gpu});
	CommandQueue queue(context, gpu);
	Program::Sources sources;

	std::vector<std::string> kernel_files;
	std::string kernel_code;
	FileUtils::read_directory(OPENCL_KERNELS_DIR, kernel_files);
	for (auto& file : kernel_files) {
		std::cout << "Reading file " << file << std::endl;
		kernel_code = FileUtils::read_file_to_string(OPENCL_KERNELS_DIR + file);
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


	struct timespec start, finish;
	std::vector<std::tuple<double, double, double>> elapsed;
	std::vector<std::pair<double, double>> similarities;
	std::tuple<DocumentClass *, DocumentClass *, DocumentClass *> class_graphs;
	GraphComparator<std::string, std::string> gComp;
	GraphSimilarity gSim;
	map<std::string, double> simComps;
	std::string topics_dir(argv[1]);
	unsigned int cnt = 0;
	std::string tmp;
	std::vector<std::string> topics;

	FileUtils::read_directory(topics_dir, topics);
	elapsed.resize(topics.size());
	similarities.resize(topics.size());
	if (topics_dir.back() != '/') {
		topics_dir += "/";
	}

	for (auto& topic : topics) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		std::get<0>(class_graphs) = construct_class_graph_serial(topics_dir + topic, true);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		std::get<0>(elapsed[cnt]) = compute_elapsed_time(&finish, &start);

		clock_gettime(CLOCK_MONOTONIC, &start);
		std::get<1>(class_graphs) = construct_class_graph_serial(topics_dir + topic);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		std::get<1>(elapsed[cnt]) = compute_elapsed_time(&finish, &start);

		clock_gettime(CLOCK_MONOTONIC, &start);
		std::get<2>(class_graphs) = construct_class_graph_parallel(topics_dir + topic, &context, &queue, &program);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		std::get<2>(elapsed[cnt]) = compute_elapsed_time(&finish, &start);

		gSim = gComp.compare(*(std::get<0>(class_graphs)), *(std::get<1>(class_graphs)));
		simComps = gSim.getSimilarityComponents();
		similarities[cnt].first = simComps["valueSimilarity"];

		gSim = gComp.compare(*(std::get<0>(class_graphs)), *(std::get<2>(class_graphs)));
		simComps = gSim.getSimilarityComponents();
		similarities[cnt].second = simComps["valueSimilarity"];

		delete std::get<0>(class_graphs);
		delete std::get<1>(class_graphs);
		delete std::get<2>(class_graphs);

		++cnt;

	}

	std::cout << std::endl << "Topic\t\t\tSerial Precise\t\tSerial Non-Precise\tOpenCL\t\t\t"
		<< "VS(Non-Precise)\t\tVS(OpenCL)\t\t" << std::endl;
	std::cout << std::setprecision(6) << std::fixed;
	for (unsigned int i=0; i<cnt; i++) {
		//tmp = boost::lexical_cast<std::string>(elapsed[i].first);
		std::cout << topics[i] << ((topics[i].length() < 8) ? "\t\t\t" : "\t\t")
			<< std::get<0>(elapsed[i]) << "\t\t" << std::get<1>(elapsed[i]) << "\t\t"
		       	<< std::get<2>(elapsed[i]) << "\t\t" << similarities[i].first << "\t\t"
			<< similarities[i].second << std::endl;
	}

}

