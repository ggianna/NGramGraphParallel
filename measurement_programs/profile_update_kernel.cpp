#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <CL/cl.hpp>

#include <dirent.h>
#include <sys/types.h>

using namespace cl;
using namespace std;


std::string read_file_to_string(const std::string& fileName) {
	ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);
	ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, ios::beg);

	vector<char> bytes(fileSize);
	ifs.read(&bytes[0], fileSize);

	return std::string(&bytes[0], fileSize);
}

double compute_elapsed_time(struct timespec *start, struct timespec *finish) {
	double result = finish->tv_sec - start->tv_sec;
	result += (finish->tv_nsec - start->tv_nsec) / 1000000000.0;

	return result;
}

Platform getPlatform() {
    /* Returns the first platform found. */
    std::vector<Platform> all_platforms;
    Platform::get(&all_platforms);

    if (all_platforms.size()==0) {
	    std::cout << "No platforms found. Check OpenCL installation!\n";
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
	    std::cout << "No devices found. Check OpenCL installation!\n";
        exit(1);
    }

    if (display) {
        for (unsigned int j=0; j<all_gpus.size(); j++)
            printf("Device %d: %s\n", j, all_gpus[j].getInfo<CL_DEVICE_NAME>().c_str());
    }
    return all_gpus[i];
}

int main(int argc, char **argv) {
	if (argc != 4) {
		std::cout << "Usage: profile_update_kernel <update_kernel_file> <table_size> <iters_no>\n";
		exit(1);
	}
	int iters;
	unsigned int table_size;
	std::istringstream ss1(argv[2]);
	if (!(ss1 >> table_size)) {
		std::cerr << "Invalid table size: " << argv[2] << "\n";
		exit(1);
	}
	std::istringstream ss2(argv[3]);
	if (!(ss2 >> iters)) {
		std::cerr << "Invalid iterations number: " << argv[3] << "\n";
		exit(1);
	}

	Platform platform = getPlatform();
	Device gpu = getDevice(platform, CL_DEVICE_TYPE_GPU, 0);
	Context context({gpu});
	Program::Sources sources;
	std::string kernel_code = read_file_to_string(argv[1]);
	sources.push_back({kernel_code.c_str(), kernel_code.length()});
	Program program(context, sources);
	if (program.build() != CL_SUCCESS) {
		std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu) << std::endl;
		exit(1);
	}
	Kernel kernel(program, "update_ngg");
	CommandQueue queue(context, gpu);

	float *arr1 = new float[table_size]();
	float *arr2 = new float[table_size]();
	unsigned char *flags = new unsigned char[table_size]();
	float l = 1.0;
	double allocationCounter = 0.0, writeCounter = 0.0, executeCounter = 0.0, totalCounter;
	double allocPercent, writePercent, executePercent;
	struct timespec start, finish, totalStart, totalFinish;
	std::vector<Buffer> buffers(0);

	for (unsigned int i=0; i<table_size; i++) {
		if (i%2 == 0) {
			arr1[i] = 1.0;
		}
		else {
			arr2[i] = 1.0;
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &totalStart);
	for (int i=0; i<iters; i++) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		buffers.push_back(Buffer(context, CL_MEM_READ_WRITE, sizeof(float)*table_size));
		buffers.push_back(Buffer(context, CL_MEM_READ_ONLY, sizeof(float)*table_size));
		buffers.push_back(Buffer(context, CL_MEM_WRITE_ONLY, sizeof(unsigned char)*table_size));
		clock_gettime(CLOCK_MONOTONIC, &finish);
		allocationCounter += compute_elapsed_time(&start, &finish);

		clock_gettime(CLOCK_MONOTONIC, &start);
		queue.enqueueWriteBuffer(buffers[0], CL_TRUE, 0, sizeof(float)*table_size, arr1);
		queue.enqueueWriteBuffer(buffers[1], CL_TRUE, 0, sizeof(float)*table_size, arr2);
		queue.enqueueWriteBuffer(buffers[2], CL_TRUE, 0, sizeof(unsigned char)*table_size, flags);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		writeCounter += compute_elapsed_time(&start, &finish);

		kernel.setArg(0,buffers[0]);
		kernel.setArg(1,buffers[1]);
		kernel.setArg(2,buffers[2]);
		kernel.setArg(3,table_size);
		kernel.setArg(4,l);

		clock_gettime(CLOCK_MONOTONIC, &start);
		queue.enqueueNDRangeKernel(kernel, NullRange, NDRange(256), NDRange(32));
		if (queue.finish() != CL_SUCCESS) {
			std::cout << "CommandQueue::finish() returned error\n";
			exit(1);
		}
		clock_gettime(CLOCK_MONOTONIC, &finish);
		executeCounter += compute_elapsed_time(&start, &finish);

		queue.enqueueReadBuffer(buffers[2], CL_TRUE, 0, sizeof(unsigned char)*table_size, flags);

		buffers.clear();
	}
	clock_gettime(CLOCK_MONOTONIC, &totalFinish);
	totalCounter = compute_elapsed_time(&totalStart, &totalFinish);

	allocPercent = (allocationCounter / totalCounter) * 100;
	writePercent = (writeCounter / totalCounter) * 100;
	executePercent = (executeCounter / totalCounter) * 100;

	std::cout << "BufAlloc\tBufWrite\tKernelExec\tTotal\n";
	std::cout << std::setprecision(3) << std::fixed << allocationCounter << " sec\t"
		<< writeCounter << " sec\t" << executeCounter << " sec\t" << totalCounter << " sec" << std::endl;
	std::cout << std::setprecision(0) << std::fixed << allocPercent << "%\t\t"
		<< writePercent << "%\t\t" << executePercent << "%\n";


	/*
	for (unsigned int i=0; i<table_size; i++) {
		if (((i%2 == 1) && (flags[i] == 0)) || ((i%2 == 0) && (flags[i] != 0))) {
			std::cout << "WRONG!\n";
			exit(1);
		}
	}

	std::cout << "RIGHT!\n";
	*/

	return 0;
}



