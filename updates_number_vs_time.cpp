#include <iostream>
#include <string>
#include <vector>
#include <tuple>

#include "NGramGraph.hpp"
#include "ProximityApproach.hpp"
#include "DocumentClass.hpp"
#include "FileUtils.hpp"


double compute_elapsed_time(struct timespec *finish, struct timespec *start) {
	double result = finish->tv_sec - start->tv_sec;
	result += (finish->tv_nsec - start->tv_nsec) / 1000000000.0;

	return result;
}


std::vector<double> heart(std::string topic_dir) {
	unsigned int base = 0;
	struct timespec start, finish;
	std::vector<double> times(10, 0.0);
	std::vector<std::string> files;
	bool end = false;
	DocumentClass *class_graph = new DocumentClass();
	NGramGraph *sample_ngg;
	StringSplitter splitter;
	StringPayload payload;
	ProximityApproach *approach = new SymmetricApproach();
	FileUtils::read_directory(topic_dir, files);

	for (int i=0; i<10; i++) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		for (unsigned int j = base; j < base + 100; j++) {
			if (j >= files.size()) {
				end = true;
				break;
			}
			payload.setPayload(FileUtils::read_file_to_string(topic_dir + files[j]));
			sample_ngg = new NGramGraph(nullptr, &splitter, &payload, 4, approach);
			sample_ngg->createGraph();
			class_graph->update(sample_ngg, true);
			delete sample_ngg;
		}
		clock_gettime(CLOCK_MONOTONIC, &finish);

		for (int k=i; k<10; k++) {
			times[k] += compute_elapsed_time(&finish, &start);
		}

		if (end) break;
		base += 100;
		std::cout << "First " << base << " done\n";
	}

	std::cout << std::endl;

	return times;
}


int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <topic_dir>\n";
		exit(1);
	}
	std::string topic_dir(argv[1]);
	if (topic_dir.back() != '/') {
		topic_dir += "/";
	}

	std::vector<double> times = heart(topic_dir);
	std::cout << std::setprecision(3) << std::fixed;
	std::cout << "#Updates\t" << "Time(sec)" << std::endl;
	for (int i = 1; i <= 10; i++) {
		std::cout << i*100 << "\t\t" << times[i-1] << std::endl;
	}

	return 0;
}
