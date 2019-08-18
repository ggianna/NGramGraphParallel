#include <fstream>
#include <string>
#include <tuple>
#include <algorithm>
#include <stdlib.h> //system()

#include "DocumentClass.hpp"
#include "Atom.hpp"
#include "StringSplitter.hpp"
#include "StringPayload.hpp"
#include "ProximityApproach.hpp"
#include "FileUtils.hpp"


DocumentClass::DocumentClass() : NGramGraph(), numberOfConstituents(0), updateOp(this, nullptr, 1.0) {}


void DocumentClass::restoreFromFile(std::string filename) {
	ifstream in(filename);
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
		addEdge(aSource, aTarget, weight);
	}

	in.close();
}


void DocumentClass::cache(std::string dir, std::string fname) {
	if (dir.back() != '/') {
		dir += "/";
	}
	std::string cmd("mkdir -p " + dir);
	system(cmd.c_str());
	std::ofstream output_file;
	std::string filepath = dir + fname;
	output_file.open(filepath, std::ofstream::out | std::ofstream::trunc);
	std::string temp;

	output_file << std::setprecision(9) << std::fixed;
	for (auto& pair : getEdgeNameToWeightMap()) {
		temp = pair.first;
		// Replace newline character with unprintable character <ESC>, so when restoring, file can be parsed line by line
		std::replace(temp.begin(), temp.end(), '\n', '\x1B'); 
		output_file << pair.second << " " << temp << std::endl;
	}

	output_file.close();
}


void DocumentClass::build(std::string dir) {
	if (dir.back() != '/') {
		dir += "/";
	}
	std::vector<std::string> docs;
	NGramGraph *sample_ngg;
	StringPayload payload;
	StringSplitter splitter;
	ProximityApproach *approach = new SymmetricApproach();
	FileUtils::read_directory(dir, docs);

	for (const auto& doc : docs) {
		// Get the contents of the file into a string and set the spayload with it.
		payload.setPayload(FileUtils::read_file_to_string(dir + doc));

		// Create a temporary NGramGraph object representing the current document.
		sample_ngg = new NGramGraph(nullptr, &splitter, &payload, 4, approach);
		sample_ngg->createGraph();

		// Update the DocumentClass object with the above NGramGraph object.
		update(sample_ngg);
		delete sample_ngg;
	}

	delete approach;
}


void DocumentClass::update(NGramGraph *newDoc, bool precise) {
	updateOp.setPrecise(precise);
	numberOfConstituents += 1;
	float new_l = computeLearningFactor();
	updateOp.setLearningFactor(new_l);
	updateOp.setDocumentOperand(newDoc);
	updateOp.apply();
}


void DocumentClass::update(NGramGraph *newDoc, double l, bool precise) {
	updateOp.setPrecise(precise);
	numberOfConstituents += 1;
	updateOp.setLearningFactor(l);
	updateOp.setDocumentOperand(newDoc);
	updateOp.apply();
}


float DocumentClass::computeLearningFactor() {
	return ( 1 - ( (numberOfConstituents - 1) / float(numberOfConstituents) ) );
}
