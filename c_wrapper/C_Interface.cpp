#include <cstdlib>
#include "C_Interface.h"
#include "C_NGramGraphConfiguration.h"
#include <algorithm>
#ifdef __cplusplus
extern "C"{
#endif


void ngg_construct(int text_id, const char* text){
	std::string s = std::string(text);
	StringPayload p(s);
	NGramGraph NGG(nullptr, &stringSplitter, &p, NGRAMSIZE_VALUE, approach);
	NGG.createGraph();
	if(text_id==0 && !NGramGraphDB.empty()){
		NGramGraphDB.push_back(NGG);
		std::swap(NGramGraphDB.front(), NGramGraphDB.back());
		NGramGraphDB.pop_back();
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
	while (text_id != num_graphs) { 
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
	NGramGraph NGG(nullptr, &stringSplitter, &p, NGRAMSIZE_VALUE, approach);
	NGG.createGraph();
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

double** new_square_mat(int n){
	double** M = new double*[n];
	for(int i = 0; i < n; i++){
		M[i] = new double[n];
		for(int j = 0; j < n; j++){
			M[i][j] = 0;
		}
	}
	return M;
}


double** ngg_compute_distance_matrix(char** docs, int ndocs){	
	double** DistMat = new_square_mat(ndocs);
	int nparts = std::ceil(double(ndocs)/MAX_MEMORY_GRAPHS);
	int ngraphs, offset, offset2;
	for(int p = 0; p < nparts ; p++){
		offset = p*MAX_MEMORY_GRAPHS;
		if(p < nparts-1)  ngraphs = MAX_MEMORY_GRAPHS; else  ngraphs = ndocs-offset;
		ngg_compute_inner_partition_distances(DistMat, docs, ngraphs, offset);
		for(int cmp_p = p+1; cmp_p < nparts; cmp_p++){
			int cmp_offset = cmp_p*MAX_MEMORY_GRAPHS;
			if(cmp_p < nparts-1) ngraphs = MAX_MEMORY_GRAPHS; else ngraphs = ndocs-cmp_offset;
			ngg_compute_cross_partition_distances(DistMat, docs, ngraphs, offset, cmp_offset);
		}
		NGramGraphDB.clear();
	}
	return DistMat;
}


void ngg_compute_cross_partition_distances( double** DistMat, char** docs, int ndocs, int offset1, int offset2){
	int nprev = NGramGraphDB.size();
	ngg_construct_graph_database(docs+offset2, nprev);
	for(int i = 0; i < nprev; i++){
		for(int j = 0 ; j < ndocs; j++){		
				DistMat[i+offset1][j+offset2] =
				DistMat[j+offset2][i+offset1]
				 	= ngg_dissimilarity(i,nprev+j);
				std::cout<<"cross D("<<i+offset1<<","<<j+offset2<<")"<<std::endl;
		}
	}
	ngg_delete_graphs(nprev,ndocs);
}


void ngg_compute_inner_partition_distances(double** DistMat, char** docs, int ndocs, int offset){
	ngg_construct_graph_database(docs, ndocs);
	double** PartitionDistMat =  ngg_compute_partition_distance_matrix(ndocs);
	for(int i = 0 ; i < ndocs; i++){
		for(int j = i ; j < ndocs; j++){
			DistMat[i+offset][j+offset] = 
			DistMat[j+offset][i+offset] =
				PartitionDistMat[i][j];
			std::cout<<"inner D("<<i+offset<<","<<j+offset<<")"<<std::endl;
		}
	}
}



double** ngg_compute_partition_distance_matrix(int nparts){
	double** PartitionDistMat = new_square_mat(nparts);
	for(int i = 0; i < nparts; i++){
		for(int j = i; j < nparts; j++){
			PartitionDistMat[i][j] =
			PartitionDistMat[j][i] 
				= ngg_dissimilarity(i,j);
		}
	}
	return PartitionDistMat;
}

void mat_vis(double** M, int n){
	for(int i = 0; i < n; i++){
		for(int j = 0 ; j < n; j++){	
			std::cout << M[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

#ifdef __cplusplus
}



#endif
