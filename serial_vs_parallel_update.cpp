#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <boost/lexical_cast.hpp>
#include <CL/cl.hpp>

#include "NGramGraph.hpp"
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


void construct_class_graph_serial(std::string dir) {
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
		class_graph->update(sample_ngg);

		delete sample_ngg;
	}

	delete class_graph;
	delete approach;
}

void construct_class_graph_parallel(std::string dir, Context *context, CommandQueue *queue, Program *program) {
	DocumentClass *class_graph = new DocumentClass(OPENCL_HASH_TABLE_SIZE);
	if (dir.back() != '/') {
		dir += "/";
	}
	class_graph->constructWithOpenCL(dir, context, queue, program);
	delete class_graph;
}


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <topics_dir>\n";
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
	std::vector<std::pair<double, double>> elapsed;
	std::string topics_dir(argv[1]);
	unsigned int cnt = 0;
	std::string tmp;
	std::vector<std::string> topics;

	FileUtils::read_directory(topics_dir, topics);
	elapsed.resize(topics.size());
	if (topics_dir.back() != '/') {
		topics_dir += "/";
	}

	for (auto& topic : topics) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		construct_class_graph_serial(topics_dir + topic);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		elapsed[cnt].first = compute_elapsed_time(&finish, &start);

		clock_gettime(CLOCK_MONOTONIC, &start);
		construct_class_graph_parallel(topics_dir + topic, &context, &queue, &program);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		elapsed[cnt].second = compute_elapsed_time(&finish, &start);

		++cnt;

	}

	std::cout << std::endl << "Topic\t\tSerial Time\t\tParallel Time" << std::endl;
	std::cout << std::setprecision(6) << std::fixed;
	for (unsigned int i=0; i<cnt; i++) {
		//tmp = boost::lexical_cast<std::string>(elapsed[i].first);
		std::cout << topics[i] << ((topics[i].length() < 8) ? "\t\t" : "\t")
			<< elapsed[i].first << "\t\t" << elapsed[i].second << std::endl;
	}

}

