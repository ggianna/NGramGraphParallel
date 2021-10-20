#include <cstdlib>
#include "C_Interface.h"
#include "C_NGramGraphConfiguration.h"
#ifdef __cplusplus
extern "C"{
#endif


void ngg_construct(int text_id, const char* text){
	std::string s = std::string(text);
	StringPayload p(s);
	NGramGraph NGG(nullptr, &stringSplitter, &p, NGRAMSIZE_VALUE, approach);
	NGG.createGraph();
	NGramGraphDB.reserve(1); 
	NGramGraphDB.push_back(NGG);
	printf("pushed back \"%s\"\n",text);
}


/** Default constructor */
void ngg_construct_graph_database(char** ptrs, int num_graphs){
	
	int text_id = 0;
	char* text = NULL;
	while (text_id != num_graphs) 
	{ 
		text = ptrs[text_id++];
		ngg_construct(text_id, text);
		
	}
}


double ngg_dissimilarity(int first_text_id, int second_text_id){

	GraphComparator<std::string, std::string> comparator;
	double cs = 
		comparator.calculateContainmentSimilarity(
			NGramGraphDB.at(first_text_id),
			NGramGraphDB.at(second_text_id)
		);
	return 1 - cs;
}




	// double computeDissimilarity(const char* msg1, const char* msg2){
	// 	Handler* handler = new Handler(valueSimilarity, 3, 2);
	// 	return handler->dissimilarity(msg1, msg2);
	// }


#ifdef __cplusplus
}



#endif
