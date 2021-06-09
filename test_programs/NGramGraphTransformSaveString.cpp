/**
 * \file main.cpp
 *
 * \author Panagiotis Fotopoulos
 *
 * \copyright Copyright 2018 NCSR Demokritos Greece \n
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. \n
 */

#include <iostream>
#include "GraphSimilarity.hpp"
#include "GraphComparator.hpp"
#include "NGramGraph.hpp"
#include "StringSplitter.hpp"
#include "ProximityApproach.hpp"
#include "InputParser.hpp"


using namespace std;

int main(int argc, char* argv[]){


	InputParser parser(argc,argv);
	
	std::string TEXT_PAYLOAD =  parser.cmdOptionExists("-s") ? parser.getCmdOption("-s") : "";
	std::string S1 =  parser.cmdOptionExists("-s1") ? parser.getCmdOption("-s1") : "";
	std::string S2 =  parser.cmdOptionExists("-s2") ? parser.getCmdOption("-s2") : "";
	
	int WINDOWSIZE =  parser.cmdOptionExists("-w") ?  atoi(parser.getCmdOption("-w").c_str()) : NGRAMSIZE_DEFAULT_VALUE;	
	int NGRAMSIZE_VALUE =  parser.cmdOptionExists("-n") ? atoi(parser.getCmdOption("-n").c_str()) : 2;

	cout << "PAYLOAD : "<<TEXT_PAYLOAD<<endl;
	cout << "WINDOW : "<<WINDOWSIZE<<endl;
	cout << "NGRAMS :"<<NGRAMSIZE_VALUE<<endl;

	StringSplitter testStringSplitter(NGRAMSIZE_VALUE);

	
	ProximityApproach* approach = new SymmetricApproach();
	
	StringPayload testStringPayload(S1);
	NGramGraph NGG1(nullptr, &testStringSplitter, &testStringPayload, WINDOWSIZE, approach);
	NGG1.createGraph();
	NGG1.printGraphviz();
	
	StringPayload testStringPayload2(S2);
	NGramGraph NGG2(nullptr, &testStringSplitter, &testStringPayload2, WINDOWSIZE, approach);
	NGG2.createGraph();
	NGG2.printGraphviz();
	
	GraphSimilarity testGraphSimilarity;
	GraphComparator<std::string, std::string> testGraphComparator;
	testGraphSimilarity = testGraphComparator.compare(NGG1, NGG2);


	map<string, double> testSimilarityComponents =  testGraphSimilarity.getSimilarityComponents();
    	cout << "Size Similarity: " << testSimilarityComponents["sizeSimilarity"] << endl;
    	cout << "Value Similarity: " << testSimilarityComponents["valueSimilarity"] << endl;
    	cout << "Normalized Value Similarity: " << testSimilarityComponents["normalizedValueSimilarity"] << endl;
    	cout << "Containment Value Similarity: " << testSimilarityComponents["containmentSimilarity"] << endl;


	}
