#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

#include "NGramGraph.hpp"
#include "DocumentClass.hpp"
#include "ProximityApproach.hpp"
#include "FileUtils.hpp"
#include "StringSplitter.hpp"
#include "StringPayload.hpp"
#include "Atom.hpp"
#include "NGGUpdateOperator.hpp"


#define CLASS_GRAPHS_DIR "./precise_class_graphs/"


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


std::string write_class_graph_to_file(DocumentClass *class_graph, std::string topic) {
	std::ofstream output_file;
	std::string fname = CLASS_GRAPHS_DIR + topic + ".cg";
	output_file.open(fname);
	std::string temp;

	output_file << std::setprecision(9) << std::fixed;
	for (auto& pair : class_graph->getEdgeNameToWeightMap()) {
		temp = pair.first;
		std::replace(temp.begin(), temp.end(), '\n', '\x1B');
		output_file << pair.second << " " << temp << std::endl;
	}

	output_file.close();

	return fname;
}


DocumentClass * read_class_graph_from_file(std::string fname) {
	DocumentClass *ret = new DocumentClass();
	NGGUpdateOperator updateOp(nullptr, nullptr, 0.0);
	ifstream in(fname);
	double weight;
	char c;
	std::string label, source, target;
	Atom<std::string> aSource, aTarget;

	while (in >> weight >> std::noskipws >> c) {
		std::getline(in, label);
		std::replace(label.begin(), label.end(), '\x1B', '\n');
		std::tie(source, target) = updateOp.extractHeadAndTailFromEdgeLabel(label);
		aSource.setData(source);
		aTarget.setData(target);
		ret->addEdge(aSource, aTarget, weight);
	}

	in.close();

	return ret;
}



int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage:" << argv[0] << " <topics_dir>\n";
		exit(1);
	}

	std::string topics_dir(argv[1]);
	if (topics_dir.back() != '/') {
		topics_dir += "/";
	}
	DocumentClass *class_graph;
	std::vector<std::string> topics;
	FileUtils::read_directory(topics_dir, topics);

	for (auto& topic : topics) {
		class_graph = construct_class_graph_serial(topics_dir + topic, true);
		write_class_graph_to_file(class_graph, topic);
		delete class_graph;
		std::cout << "Topic '" << topic << "' [OK]\n";
	}

	return 0;
}
