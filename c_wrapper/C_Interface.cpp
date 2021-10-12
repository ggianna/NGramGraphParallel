#include <cstdlib>
#include "C_Interface.h"
#include "C_HandleGraphRequest.h"
#include "GraphComparator.hpp"
#include "GraphSimilarity.hpp"
#include "GraphComparator.hpp"
#include "NGramGraph.hpp"
#include "StringSplitter.hpp"
#include "ProximityApproach.hpp"

#ifdef __cplusplus
extern "C"{
#endif


double valueDissimilarity(const char* msg1,const char* msg2){
	Handler* handler = new Handler(nullptr, 3, 2);
	std::string s1 = std::string(msg1);
	std::string s2 = std::string(msg2);
	StringSplitter stringSplitter(handler->NGRAMSIZE_VALUE);
	StringPayload p1(s1);
	StringPayload p2(s2);
	ProximityApproach* approach = new SymmetricApproach();
	NGramGraph G1(nullptr, &stringSplitter, &p1, handler->NGRAMSIZE_VALUE, approach);
	NGramGraph G2(nullptr, &stringSplitter, &p2, handler->NGRAMSIZE_VALUE, approach);
	
	G1.createGraph();
	G2.createGraph();
	GraphComparator<std::string, std::string> comparator;
	GraphSimilarity sim = comparator.compare(G1, G2);
	return 1 - sim.getSimilarityComponents()["valueSimilarity"];
}

// double computeDissimilarity(const char* msg1, const char* msg2){
// 	Handler* handler = new Handler(valueSimilarity, 3, 2);
// 	return handler->dissimilarity(msg1, msg2);
// }


#ifdef __cplusplus
}



#endif
