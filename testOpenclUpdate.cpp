#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <CL/cl.hpp>
#include <sys/types.h>
#include <dirent.h>

#include "DocumentClass.hpp"
#include "DocumentClassComponent.hpp"
#include "FileUtils.hpp"


#define OPENCL_KERNELS_DIR "./ngg_opencl_kernels/"
#define HASH_TABLE_SIZE 1000000




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


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: ./testOpenclUpdate <topics_dir>\n";
		std::cout << "<topics_dir> should be a directory containing subdirectories that contain text files. "
			  << "Each subdirectory should represent a different topic.\n";
		exit(1);
	}
	std::string topics_dir(argv[1]);
	if (topics_dir.back() != '/') {
		topics_dir += "/";
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
	/* End of OpenCL initialization */


	/* Class graph construction */
	std::cout << std::endl;
	unsigned int cnt = 0;
	std::vector<DocumentClass *> class_graphs;
	std::vector<std::string> topics;
	FileUtils::read_directory(topics_dir, topics);
	class_graphs.resize(topics.size());
	for (auto& topic : topics) {
		std::cout << "Creating class graph for topic '" << topic << "'...";
		std::cout.flush();
		//printf("Creating class graph for topic '%s'...", topic.c_str());
		class_graphs[cnt] = new DocumentClass(HASH_TABLE_SIZE);
		class_graphs[cnt]->constructWithOpenCL(topics_dir + topic + "/", &context, &queue, &program);
		std::cout << "\t[OK]\n";
		++cnt;
	}


	return 0;
}

