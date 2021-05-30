#include <iostream>
#include "NGramGraph.hpp"
#include "GraphSimilarity.hpp"
#include "GraphComparator.hpp"
#include "StringSplitter.hpp"
#include "ProximityApproach.hpp"
#include "InputParser.hpp"
#include "C_HandleGraphRequest.hpp"
using namespace std;
int getGraph(const char* msg){
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
