#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <openclParams.hpp>

#include "OclUpdatableClass.hpp"
#include "FileUtils.hpp"

#define OPENCL_KERNELS_DIR "./ngg_opencl_kernels/"

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


std::map<std::string, double> construct_class_graph_parallel(std::string dir, Context *context,
			CommandQueue *queue, Program *program, std::size_t size)
{
	std::map<std::string, double> ret;
	OclUpdatableClass *class_graph = new OclUpdatableClass(size);
	if (dir.back() != '/') {
		dir += "/";
	}
	ret = class_graph->profileClassBuilding(dir, context, queue, program);
	delete class_graph;

	return ret;
}


int main(int argc, char **argv) {
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <topics_dir> <table_sizes_file>\n"
			<< "\t<topics_dir> should be a directory containing subdirectories that contain text files.\n"
			<< "\tEach subdirectory should represent a topic.\n"
			<< "\t<table_sizes_file> should be a file, containing the table sizes to be tried, one at a line.\n";
		exit(1);
	}

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


	std::vector<std::string> topics;
	FileUtils::read_directory(topics_dir, topics);
	unsigned int size_cnt = 0, topic_cnt = 0;
	std::vector<std::vector<std::map<std::string, double>>> times;
	times.resize(topics.size());
	for (auto& t : times) {
		t.resize(sizes.size());
	}

	for (auto& topic : topics) {
		size_cnt = 0;
		for (auto& size : sizes) {
			times[topic_cnt][size_cnt] = construct_class_graph_parallel(topics_dir + topic, &context, &queue, &program, size);
			++size_cnt;
		}
		++topic_cnt;
	}

	std::vector<std::map<std::string, double>> results;
	results.resize(sizes.size());
	std::vector<std::string> tasks{"total", "componentPreparation", "openclKernel", "newEdgeCollection", "graphBuilding"};
	for (auto& r : results) {
		for (auto& t : tasks) {
			r[t] = 0.0;
		}
	}

	for (unsigned int size_i = 0; size_i < sizes.size(); size_i++) {
		for (unsigned int topic_i = 0; topic_i < topics.size(); topic_i++) {
			for (auto& task : tasks) {
				(results[size_i])[task] += (times[topic_i][size_i])[task];
			}
		}
		for (auto& task : tasks) {
			(results[size_i])[task] /= double(topics.size());
		}
	}



	std::cout << "\t\t\t";
	for (auto& task : tasks) {
		std::cout << task;
		if (task.length() < 8) std::cout << "\t\t\t";
		else if (task.length() < 16) std::cout << "\t\t";
		else std::cout << "\t";
	}
	std::cout << std::endl;

	size_cnt = 0;
	std::cout << std::setprecision(2) << std::fixed;
	for (auto& row : results) {
		std::cout << sizes[size_cnt] << ((sizes[size_cnt] < 10000000) ? "\t\t\t" : "\t\t");
		for (auto& t : tasks) {
			std::cout << row[t];
			if (t != "total") {
				std::cout << "(" << std::lround((row[t] / row["total"]) * 100) << "%)\t\t";
			}
			else {
				std::cout << "\t\t\t";
			}
		}
		std::cout << std::endl;
		++size_cnt;
	}

	return 0;
}
