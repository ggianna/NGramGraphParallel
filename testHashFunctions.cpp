#include <iostream>
#include <vector>

#include "HashFunction.hpp"
#include "NGramGraph.hpp"
#include "ProximityApproach.hpp"


size_t sizes[] = {
	1000000,
	5000000
};

#define HASH_FUNCTIONS_NUMBER 2
#define SIZES_NUMBER 2

std::string readFileToString(const std::string& fileName) {
	ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);
	ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, ios::beg);

	vector<char> bytes(fileSize);
	ifs.read(&bytes[0], fileSize);

	return std::string(&bytes[0], fileSize);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: ./testHashFunctions <document_filename>" << std::endl;
		exit(1);
	}


	StringSplitter sspliter;
	StringPayload spayload(readFileToString(argv[1]));
	ProximityApproach* approach = new SymmetricApproach();

	NGramGraph docGraph(nullptr, &sspliter, &spayload, 2, approach);
	docGraph.createGraph();

	vector<HashFunction<std::string, unsigned long> *> hFuns(HASH_FUNCTIONS_NUMBER);
	hFuns[0] = new DJB2();
	hFuns[1] = new JenkinsHash();
	vector<bool> hTable;
	unsigned long collisions, hash;
	std::string funName;

	cout << std::endl << "Hash Name\tTable Size\t#Collisions" << std::endl;
	for (int i = 0; i < HASH_FUNCTIONS_NUMBER; i++) {
		for (int j = 0; j < SIZES_NUMBER; j++) {

			hTable.clear();
			hTable.resize(sizes[j], false);
			collisions = 0;
			
			for (auto const& elem : docGraph.getEdgeNameToWeightMap()) {
				hFuns[i]->setArgument(elem.first);
				hash = hFuns[i]->apply() % sizes[j];
				if (hTable[hash] == false) {
					hTable[hash] = true;
				}
				else {
					++collisions;
				}
			}

			funName = hFuns[i]->getName();
			std::cout << funName << ((funName.length() < 8) ? "\t\t" : "\t") << sizes[j] << "\t\t" << collisions << std::endl;
		}
	}


	return 0;
}



