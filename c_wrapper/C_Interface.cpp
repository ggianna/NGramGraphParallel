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
	int text_id = 0;
	char* text = NULL;
	std::string placeholder(NGRAMSIZE_VALUE+1, ' ');
	while (text_id != num_graphs) 
	{ 
		text = ptrs[text_id++];
		if(text==nullptr || strlen(text) < NGRAMSIZE_VALUE ) {
			ngg_construct(NGramGraphDB.size(), placeholder.c_str());
		}  
		else{
			ngg_construct(NGramGraphDB.size(), text);
		}
	}
}



void ngg_delete_graphs(int offset, int num_graphs){
	for(int i = offset ; i < offset+num_graphs ; i++){
		std::swap(NGramGraphDB.at(offset), NGramGraphDB.back());
		NGramGraphDB.pop_back();
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


double** ngg_compute_distance_matrix(char** docs, int n_docs){
	int partition_size = 2;
	double** DM = new double*[n_docs];
	for(int i = 0; i < n_docs; i++){
		DM[i] = new double[n_docs];
		for(int j = 0; j < n_docs; j++){
			DM[i][j] = 0;
		}
	}

	int n_partitions = std::ceil(double(n_docs)/partition_size);
	int offset;
	for(int partition_id = 0; partition_id < n_partitions ; partition_id++){
	
		std::cout<<"compute partition "<<partition_id<<" / " << n_partitions<<std::endl;
		offset = partition_id*partition_size;
		if(partition_id < n_partitions-1){
			ngg_compute_partition(docs, partition_size, DM, offset);
		}
		else{
			ngg_compute_partition(docs, n_docs-offset, DM, offset);
		}

		int offset2;
		for(int cmp_id = partition_id+1; cmp_id < n_partitions ; cmp_id++){
			offset2 = cmp_id*partition_size;
			if(cmp_id < n_partitions-1){
				ngg_compute_cross_partition(docs, partition_size, DM, offset, offset2);
			}
			else{
				ngg_compute_cross_partition(docs, n_docs-offset2, DM, offset, offset2);
			}
		}
		NGramGraphDB.clear();
	
	}
	ngg_visualize_distance_matrix(DM,n_docs);
	return DM;
}


void ngg_compute_cross_partition(char** docs, int n_docs, double** DM, int offset1, int offset2){
	
	int n_docs_1 = NGramGraphDB.size();
	ngg_construct_graph_database(docs+offset2, n_docs);
	for(int i = 0; i < n_docs_1; i++){
		for(int j = 0 ; j < n_docs; j++){
				std::cout<<"dist("<<i+offset1<<","<<j+offset2<<")"<<std::endl;
				DM[i+offset1][j+offset2] = DM[j+offset2][i+offset1] = ngg_dissimilarity(i,n_docs_1+j);
		}
	}
	std::cout<<"delete "<<n_docs<<" graphs"<<std::endl;
	ngg_delete_graphs(n_docs_1,n_docs);
}


void ngg_compute_partition(char** docs, int n_docs, double** DM, int offset){
	ngg_construct_graph_database(docs, n_docs);
	double**PM =  ngg_compute_partition_distance_matrix(n_docs);
	for(int i = 0 ; i < n_docs; i++){
		for(int j = i ; j < n_docs; j++){
			DM[i+offset][j+offset] = DM[j+offset][i+offset] = PM[i][j];
		}
	}
}



double** ngg_compute_partition_distance_matrix(int n_parts){
	std::cout<<"compute dist "<<n_parts<< " x "<<n_parts<<" matrix"<<std::endl;
	double** DM = new double*[n_parts];
	
	for(int i = 0; i < n_parts; i++){
		DM[i] = new double[n_parts];
	}
	
	for(int i = 0; i < n_parts; i++){
		/*compute upper triangular matrix and reflect*/
		for(int j = i; j < n_parts; j++){
			DM[i][j] = DM[j][i] = ngg_dissimilarity(i,j);//assume symmetric distance
			std::cout<<"dist("<<i<<","<<j<<"):\t"<<DM[i][j]<<std::endl;
		}
	}
	// ngg_visualize_distance_matrix(DM, n_parts);
	return DM;
}

void ngg_visualize_distance_matrix(double** DM, int n_parts){
	/*visualize */
	for(int i = 0; i < n_parts; i++){
		for(int j = 0 ; j < n_parts; j++){	
			std::cout << DM[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

#ifdef __cplusplus
}



#endif
