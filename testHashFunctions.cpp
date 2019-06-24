#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sys/types.h>
#include <dirent.h>
#include "HashFunction.hpp"
#include "NGramGraph.hpp"
#include "ProximityApproach.hpp"
  
#define HASH_FUNCTIONS_NUMBER 2
#define SAMPLES_FOLDER "./textSamples/"
#define LINE_SEPARATOR "\n-------------------------------------------------"
#define TABLE_SIZE_COLUMN_LENGTH 10

void read_directory(const std::string& name, std::vector<std::string>& v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
	if (dp->d_name[0] != '.')
            v.push_back(dp->d_name);
    }
    closedir(dirp);
}

std::string readFileToString(const std::string& fileName) {
	ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);
	ifstream::pos_type fileSize = ifs.tellg();
	ifs.seekg(0, ios::beg);

	vector<char> bytes(fileSize);
	ifs.read(&bytes[0], fileSize);

	return std::string(&bytes[0], fileSize);
}

std::string tableSizeColumnElem(size_t s) {
	std::string ret = "| " + std::to_string(s);
	int c = TABLE_SIZE_COLUMN_LENGTH - ret.length();

	for (int i = 0; i < c; i++)
		ret += " ";

	ret += "|\t";
	return ret;
}

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: ./testHashFunctions <table_sizes_file>" << std::endl;
		exit(1);
	}

	std::cout << "\nThis program counts number of collisions for " 
		  << "various hash function and table size combinations.\n"
		  << "The first column of each table holds the table sizes used.\n\n";

	std::ifstream sizesFile(argv[1]);
	vector<size_t> sizes(0);
	size_t temp_size;
	while (sizesFile >> temp_size) {
		sizes.push_back(temp_size);
	}

	std::vector<std::string> fileNames;
	read_directory(SAMPLES_FOLDER, fileNames);

	vector<HashFunction<std::string, unsigned long> *> hFuns(HASH_FUNCTIONS_NUMBER);
	hFuns[0] = new DJB2();
	hFuns[1] = new JenkinsHash();
	vector<bool> hTable;
	unsigned long collisions, hash, keysNo;
	std::string funName;

	StringSplitter sspliter;
	StringPayload spayload;
	ProximityApproach* approach = new SymmetricApproach();
	NGramGraph *docGraph;


	for (const std::string& fileName : fileNames) {
		spayload.setPayload(readFileToString(SAMPLES_FOLDER + fileName));
		docGraph = new NGramGraph(nullptr, &sspliter, &spayload, 2, approach);
		docGraph->createGraph();
		keysNo = docGraph->numberOfEdges();
		cout << std::endl << "Measurments for sample file '" << fileName << "' (" << keysNo << " n-grams)" << std::endl;

		std::cout << "\t\t";
		for (auto& h : hFuns) {
			funName = h->getName();
			std::cout << funName << ((funName.length() < 8) ? "\t\t" : "\t");
		}
		std::cout << LINE_SEPARATOR;
		for (unsigned int i = 0; i < sizes.size(); i++) {
			std::cout << "\n" << tableSizeColumnElem(sizes[i]);
			for (unsigned int j = 0; j < HASH_FUNCTIONS_NUMBER; j++) {

				hTable.clear();
				hTable.resize(sizes[i], false);
				collisions = 0;
			
				for (auto const& elem : docGraph->getEdgeNameToWeightMap()) {
					hFuns[j]->setArgument(elem.first);
					hash = ( hFuns[j]->apply() ) % sizes[i];
					if (hTable[hash] == false) {
						hTable[hash] = true;
					}
					else {
						++collisions;
					}
				}

				funName = hFuns[j]->getName();
				std::cout << collisions << "\t\t";
			}
			std::cout << "|";
			std::cout << LINE_SEPARATOR;
		}

		delete docGraph;
		std::cout << "\n\n";
	}

	return 0;
}



