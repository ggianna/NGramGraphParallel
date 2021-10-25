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
}


/** Default constructor */
void ngg_construct_graph_database(char** ptrs, int num_graphs){
	std::cout << "construct graph database" << endl;		
	int text_id = 0;
	char* text = NULL;
	while (text_id != num_graphs) 
	{ 
		std::cout<<"text id:"<<text_id<<std::endl;
		text = ptrs[text_id++];
		if(text==NULL || strlen(text) < NGRAMSIZE_VALUE ) {
			ngg_construct(text_id, (char*)new (nothrow) char[NGRAMSIZE_VALUE]());
			continue;
		}
		ngg_construct(text_id, text);
		
	}
}


// /** Default constructor */
// void ngg_store_graph_database(const char* filename){
// 	std::ofstream output_file(filename);
//  	std::ostream_iterator<std::vector<NGramGraph>> output_iterator(output_file, "\n");
// 	std::copy(NGramGraphDB.begin(), NGramGraphDB.end(), output_iterator);
// 	output_file.close();
// }


// /** Default constructor */
// void ngg_load_graph_database(const char* filename){
	
// 	std::ifstream input_file(filename);
// 	std::istream_iterator<NGramGraph> start(input_file), end;
// 	NGramGraphDB.clear();
// 	std::vector<NGramGraph>  newDB(start, end);
// 	size_t db_size = newDB.size();
// 	std::cout << "Read " << db_size << " numbers" << std::endl;
// 	if (db_size > 1){
// 		std::copy(newDB.begin(), newDB.end(), NGramGraphDB);
// 	}
// 	input_file.close();
// }



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
