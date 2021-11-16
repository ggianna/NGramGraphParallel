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
		NGramGraphDB.clear();
	}
	// std::cout << "construct graph database" << endl;		
	int text_id = 0;
	char* text = NULL;
	std::string placeholder(NGRAMSIZE_VALUE+1, ' ');
	while (text_id != num_graphs+1) 
	{ 
		// std::cout<<"text id:"<<text_id<<std::endl;
		text = ptrs[text_id++];
		if(text==nullptr || strlen(text) < NGRAMSIZE_VALUE ) {
			// std::cout<<"construct placeholder"<<std::endl;
			ngg_construct(text_id, placeholder.c_str());
		}  
		else{
			ngg_construct(text_id, text);
		}
	}
}



/** Default constructor */
void ngg_serialize( const char* text){
	std::string s = std::string(text);
	StringPayload p(s);
	// std::cout<<"constructing "<<text<<std::endl;
	NGramGraph NGG(nullptr, &stringSplitter, &p, NGRAMSIZE_VALUE, approach);
	NGG.createGraph();
	std::cout<<"Serialization: create NGG for text:"<<s<<std::endl;

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


void ngg_compute_distance_matrix(char** docs, int n_docs){
	int partition_size = 1000;
	int n_partitions = std::round(n_docs/partition_size);
	for(int partition_id = 0; partition_id < n_partitions-1 ; partition_id++){
		std::cout<<"compute partition "<<partition_id<<endl;
		ngg_compute_partition(docs+partition_id*partition_size, partition_size);
	}
	std::cout<<"compute last partition"<<endl;
	ngg_compute_partition(docs+(n_partitions-1)*partition_size, n_docs-(n_partitions*partition_size));
}



void ngg_compute_partition(char** docs, int n_docs){
	ngg_construct_graph_database(docs, n_docs);
	ngg_compute_partition_distance_matrix(n_docs);
}



double** ngg_compute_partition_distance_matrix(int n_parts){
	std::cout<<"compute dist "<<n_parts<< " x "<<n_parts<<" matrix"<<std::endl;
	double** DistMat = new double*[n_parts];
	
	for(int i = 0; i < n_parts; i++){
		DistMat[i] = new double[n_parts];
	}
	
	for(int i = 0; i < n_parts; i++){
		/*compute upper triangular matrix and reflect*/
		for(int j = i; j < n_parts; j++){
			DistMat[i][j] = DistMat[j][i] = ngg_dissimilarity(i,j);//assume symmetric distance
			std::cout<<"dist("<<i<<","<<j<<"):\t"<<DistMat[i][j]<<std::endl;
		}
	}
	// ngg_visualize_distance_matrix(DistMat, n_parts);
	return DistMat;
}

void ngg_visualize_distance_matrix(double** DistMat, int n_parts){
	/*visualize */
	for(int i = 0; i < n_parts; i++){
		for(int j = 0 ; j < n_parts; j++){	
			std::cout << DistMat[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

#ifdef __cplusplus
}



#endif
