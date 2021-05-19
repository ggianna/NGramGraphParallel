#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <CL/opencl.hpp>

#include "ProximityApproach.hpp"
#include "OclUpdatableClass.hpp"
#include "FileUtils.hpp"

#define OPENCL_KERNELS_DIR "./opencl_kernels/"

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


double compute_elapsed_time(struct timespec *start, struct timespec *finish) {
	double result = finish->tv_sec - start->tv_sec;
	result += (finish->tv_nsec - start->tv_nsec) / 1000000000.0;

	return result;
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

	struct timespec start, finish;
	OclUpdatableClass *class_graph;
	std::vector<std::string> topics;
	FileUtils::read_directory(topics_dir, topics);
	unsigned int size_cnt = 0, topic_cnt = 0;
	std::vector<std::vector<double>> times;
	times.resize(topics.size());
	for (auto& t : times) {
		t.resize(sizes.size());
	}

	for (auto& size : sizes) {
		topic_cnt = 0;
		for (auto& topic : topics) {
			clock_gettime(CLOCK_MONOTONIC, &start);
			class_graph = construct_class_graph_parallel(topics_dir + topic, &context, &queue, &program, size);
			clock_gettime(CLOCK_MONOTONIC, &finish);
			times[topic_cnt][size_cnt] = compute_elapsed_time(&start, &finish);

			delete class_graph;
			++topic_cnt;
		}
		++size_cnt;
	}

	std::cout << "\n\t\t";
	for (auto& size : sizes) {
		std::cout << size << ((size < 10000000) ? "\t\t" : "\t");
	}
	std::cout << std::endl;

	topic_cnt = 0;
	std::cout << std::setprecision(3) << std::fixed;
	for (auto& row : times) {
		std::cout << topics[topic_cnt] << ((topics[topic_cnt].length() < 8) ? "\t\t" : "\t");
		for (auto& time : row) {
			std::cout << time << "\t\t";
		}
		std::cout << std::endl;
		++topic_cnt;
	}

	return 0;
}
