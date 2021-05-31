#include "C_HandleGraphRequest.h"
#include <stdio.h>
#include <iostream>
#include "/home/ngialitsis/search/NGramGraphParallel/include/NGramGraph.hpp"
#include "/home/ngialitsis/search/NGramGraphParallel/include/StringSplitter.hpp"
#include "/home/ngialitsis/search/NGramGraphParallel/include/Splitter.hpp"
#include "/home/ngialitsis/search/NGramGraphParallel/include/Payload.hpp"
#include "/home/ngialitsis/search/NGramGraphParallel/include/Atom.hpp"
#include "ProximityApproach.hpp"

using namespace std;


Handler::Handler(){}

int Handler::getGraph(const char* msg){
	std::string TEXT_PAYLOAD = std::string(msg);
	int WINDOWSIZE =  NGRAMSIZE_DEFAULT_VALUE;
	int NGRAMSIZE_VALUE =  2;

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
	
	return 1;
}

