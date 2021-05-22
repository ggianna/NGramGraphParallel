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
#include "UniqueVertexGraph.hpp"
#include "StringAtom.hpp"
#include "StringSplitter.hpp"
#include "ProximityApproach.hpp"
#include "DocumentClass.hpp"
#include "NGGMergeOperator.hpp"


using namespace std;

int main(int argc, char* argv[]){

	std::string TEXT_PAYLOAD;
	int NGRAMSIZE_VALUE=NGRAMSIZE_DEFAULT_VALUE;
	int WINDOWSIZE=2;

	for( int i = 0 ; i <= argc-1; i++ ){
		std::string arg(argv[i]);
		if (arg.compare("-s") == 0 or arg.compare("-S")==0){
			TEXT_PAYLOAD = std::string(argv[i+1]);
		}
		else if (arg.compare("-w") == 0 or arg.compare("-W")==0){
			WINDOWSIZE = atoi(argv[i+1]);
		}
		else if (arg.compare("-n") == 0 or arg.compare("-N")==0){
			NGRAMSIZE_VALUE = atoi(argv[i+1]);
		}
		else{;}
	}
    cout << "PAYLOAD : "<<TEXT_PAYLOAD<<endl;
    cout << "WINDOW : "<<WINDOWSIZE<<endl;
    cout << "NGRAMS :"<<NGRAMSIZE_VALUE<<endl;

    StringSplitter testStringSplitter(NGRAMSIZE_VALUE);
    
    StringPayload testStringPayload(TEXT_PAYLOAD);
    
    ProximityApproach* approach = new SymmetricApproach();

    NGramGraph testNGramGraph(nullptr, &testStringSplitter, &testStringPayload, 2, approach);
    testNGramGraph.createGraph();
    testNGramGraph.printGraphviz();
    testNGramGraph.printGraphvizToFile("out.dot");
}
