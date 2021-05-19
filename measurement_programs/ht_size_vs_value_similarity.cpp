#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>
#ifdef __APPLE__
   #include <OpenCL/opencl.hpp>
#else
   #include <CL/opencl.hpp>
#endif

#include "DocumentClass.hpp"
#include "OclUpdatableClass.hpp"
#include "NGGUpdateOperator.hpp"
#include "Atom.hpp"
#include "GraphSimilarity.hpp"
#include "GraphComparator.hpp"
#include "FileUtils.hpp"


#define OPENCL_KERNELS_DIR "./opencl_kernels/"
#define SERIAL_CLASS_GRAPHS_RELATIVE_DIR "class_graphs_serial/"


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


OclUpdatableClass * construct_class_graph_parallel(std::string dir, Context *context, CommandQueue *queue, Program *program, std::size_t size)
{
	OclUpdatableClass *class_graph = new OclUpdatableClass(size);
	if (dir.back() != '/') {
		dir += "/";
	}
	class_graph->buildClass(dir, context, queue, program);

	return class_graph;
}


bool prompt_for_char(std::string prompt, char& readch) {
	std::string tmp;
	std::cout << prompt << std::endl;
	if (std::getline(std::cin, tmp)) {
		if (tmp.length() == 1) {
			readch = tmp[0];
		}
		else {
			readch = '\0';
		}
		return true;
	}
	return false;
}


int main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <topics_dir> <table_sizes_file>\n"
			<< "\t<topics_dir> should be a directory containing subdirectories that contain text files.\n"
			<< "\tEach subdirectory should represent a topic.\n"
			<< "\t<table_sizes_file> should be a file, containing the table sizes to be tried, one at a line.\n";
		exit(1);
	}

	std::string project_dir(argv[0]);
	project_dir = project_dir.substr(0, project_dir.find_last_of("/")); // Removes the program name from the filepath
	project_dir = project_dir.substr(0, project_dir.find_last_of("/")+1); // Removes the 'measurement_programs' entry from the path
	std::string serial_class_graphs_dir(project_dir + SERIAL_CLASS_GRAPHS_RELATIVE_DIR);

	std::string topics_dir(argv[1]);
	if (topics_dir.back() != '/') {
		topics_dir += "/";
	}

	std::vector<std::size_t> sizes(0);
	std::size_t tempSize;
	std::ifstream sizesFile(argv[2]);
	while (sizesFile >> tempSize) {
		sizes.push_back(tempSize);
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

	OclUpdatableClass *ocl_class_graph;
	DocumentClass *correct_class_graph;
	std::vector<std::string> topics;
	FileUtils::read_directory(topics_dir, topics);
	GraphComparator<std::string, std::string> gComp;
	GraphSimilarity gSim;
	map<std::string, double> simComps;
	std::vector<std::vector<double>> similarities;
	similarities.resize(topics.size());
	unsigned int size_cnt = 0, topic_cnt = 0;
	for (auto& s : similarities) {
		s.resize(sizes.size());
	}

	char answer;
	bool skip = false;
	std::vector<std::string> skipped_topics;
	for (auto& topic : topics) {
		size_cnt = 0;
		if (!FileUtils::fileExists(serial_class_graphs_dir + topic + ".cg")) {
			while (prompt_for_char("No cached class graph for topic '" + topic + "'. Compute one now? [y/n]", answer)) {
				if (answer == 'y' || answer == 'Y') {
					correct_class_graph = new DocumentClass();
					correct_class_graph->build(topics_dir + topic + "/");
					correct_class_graph->cache(serial_class_graphs_dir, topic + ".cg");
					break;
				}
				else if (answer == 'n' || answer == 'N') {
					std::cout << "Topic '" + topic + "' will be skipped.\n";
					skip = true;
					break;
				}
			}
		}				
		else {
			correct_class_graph = new DocumentClass();
			correct_class_graph->restoreFromFile(serial_class_graphs_dir + topic + ".cg");
		}
		if (skip) {
			skip = false;
			skipped_topics.push_back(topic);
			++topic_cnt;
			continue;
		}
		for (auto& size : sizes) {
			ocl_class_graph = construct_class_graph_parallel(topics_dir + topic, &context, &queue, &program, size);
			gSim = gComp.compare(*ocl_class_graph, *correct_class_graph);
			simComps = gSim.getSimilarityComponents();
			similarities[topic_cnt][size_cnt] = simComps["valueSimilarity"];

			delete ocl_class_graph;
			++size_cnt;
		}
		delete correct_class_graph;
		++topic_cnt;
	}

	if (topics.size() > skipped_topics.size()) {
		std::cout << "\n\t\t";
		for (auto& size : sizes) {
			std::cout << size << ((size < 10000000) ? "\t\t" : "\t");
		}
		std::cout << std::endl;
	}
	else {
		std::cout << "\nAll available topics are skipped. Nothing to do. Exiting.\n";
		exit(0);
	}

	topic_cnt = 0;
	std::cout << std::setprecision(5) << std::fixed;
	for (auto& row : similarities) {
		if (std::find(skipped_topics.begin(), skipped_topics.end(), topics[topic_cnt]) == skipped_topics.end()) {
			std::cout << topics[topic_cnt] << ((topics[topic_cnt].length() < 8) ? "\t\t" : "\t");
			for (auto& similarity : row) {
				std::cout << similarity << "\t\t";
			}
			std::cout << std::endl;
		}
		++topic_cnt;
	}

	return 0;
}
