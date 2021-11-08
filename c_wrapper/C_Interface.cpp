#include <cstdlib>
#include "C_Interface.h"
#include "C_NGramGraphConfiguration.h"
#include <algorithm>
#ifdef __cplusplus
extern "C"{
#endif


void ngg_construct(int text_id, const char* text){
	std::string s = std::string(text);
	// std::cout<<"constructing "<<text<<std::endl;
	StringPayload p(s);
	NGramGraph NGG(nullptr, &stringSplitter, &p, NGRAMSIZE_VALUE, approach);
	NGG.createGraph();
	if(text_id==0 && !NGramGraphDB.empty()){
		std::cout<<"DB contains "<<NGramGraphDB.size()<<" elements"<<std::endl; 
		NGramGraphDB.push_back(NGG);
		std::swap(NGramGraphDB.front(), NGramGraphDB.back());
		NGramGraphDB.pop_back();
		std::cout<<"Pushed - DB contains "<<NGramGraphDB.size()<<" elements"<<std::endl; 
	}
	else{
		NGramGraphDB.reserve(1); 
		NGramGraphDB.push_back(NGG);
	}
}


/** Default constructor */
void ngg_construct_graph_database(char** ptrs, int num_graphs){
	if(!NGramGraphDB.empty()){
		return;
	}
	std::cout << "construct graph database" << endl;		
	int text_id = 0;
	char* text = NULL;
	std::string placeholder(NGRAMSIZE_VALUE+1, ' ');
	while (text_id != num_graphs+1) 
	{ 
		std::cout<<"text id:"<<text_id<<std::endl;
		text = ptrs[text_id++];
		if(text==nullptr || strlen(text) < NGRAMSIZE_VALUE ) {
			std::cout<<"construct placeholder"<<std::endl;
			ngg_construct(text_id, placeholder.c_str());
		}  
		else{
			ngg_construct(text_id, text);
		}
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

	std::cout<<"compute ngg dissimilarity between "<< first_text_id<<", "<<second_text_id<<std::endl;
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
