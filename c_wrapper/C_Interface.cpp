#include <cstdlib>
#include "C_Interface.h"
#include "C_NGramGraphConfiguration.h"
#include <algorithm>
#ifdef __cplusplus
extern "C"{
#endif




void ngg_construct(int text_id, const char* text){
	std::string s;
	if(!text || strlen(text) < NGRAMSIZE_VALUE){
		s = std::string(100, ' ');
	} else {
		s = std::string(text);
	}
	StringPayload p(s);
	NGramGraph NGG(nullptr, &stringSplitter, &p, NGRAMSIZE_VALUE, approach);
	NGG.createGraph();
	if(text_id==0 && !NGramGraphCache.empty()){
		NGramGraphCache.push_back(NGG);
		std::swap(NGramGraphCache.front(), NGramGraphCache.back());
		NGramGraphCache.pop_back();
	}
	else{
		NGramGraphCache.reserve(1); 
		NGramGraphCache.push_back(NGG);
	}
}


/** Default constructor */

void make_cache_graphs(char** ptrs, int ngraphs){	
	std::string placeholder(NGRAMSIZE_VALUE+1, ' ');
	for(int i = 0 ; i < ngraphs; i++){
		const char* text = ptrs[i];
		if(text==nullptr || strlen(text) <= NGRAMSIZE_VALUE)
			text = placeholder.c_str();
		ngg_construct(NGramGraphCache.size(), text);
	}
	
}
void print_state (const std::ios& stream) {
  std::cout << " good()=" << stream.good();
  std::cout << " eof()=" << stream.eof();
  std::cout << " fail()=" << stream.fail();
  std::cout << " bad()=" << stream.bad();
  std::cout << std::endl;
}

void decerialize(const char* binfile){
	if(precomputedDistanceMatrix && precomputedDistanceMatrix->distances) return;
	std::ifstream is(binfile, std::ios::binary);
	while(!is.good()){print_state(is);};
	cereal::BinaryInputArchive iarchive(is);
	SerializableDistMat SDM;
	iarchive(SDM);
	precomputedDistanceMatrix = new_distance_matrix(SDM.n);
	int row_id = 0;
	for (std::vector<std::vector<double>>::const_iterator row = SDM.distances.begin(); row != SDM.distances.end(); ++row){
		int cell_id = 0;
		for (std::vector<double>::const_iterator cell = row->begin(); cell != row->end(); ++cell){
			precomputedDistanceMatrix->distances[row_id][cell_id++] = *cell;
		}	
		row_id++;
	}
	std::cout<<"initialize with "<<row_id<<" columns"<<std::endl;
	
}

void cerealize(DistMat* DM, const char* binfile){
	std::ofstream os(binfile, std::ios::binary);
	cereal::BinaryOutputArchive oarchive( os );
	SerializableDistMat S(DM);
	oarchive(S);
}


void uncache_graphs(int offset, int ngraphs){
	for(int i = offset ; i < offset+ngraphs ; i++){
		std::swap(NGramGraphCache.at(offset), NGramGraphCache.back());
		NGramGraphCache.pop_back();
	}		
}

double get_precomputed_distance_if_exists(int first_text_id, int second_text_id){
	if( first_text_id != 0 && second_text_id != 0){
		return precomputedDistanceMatrix->distances[first_text_id-1][second_text_id-1];
	}
	else{
		return -1;
	}
}



double ngg_dissimilarity(int first_text_id, int second_text_id){
	
	GraphComparator<std::string, std::string> comparator;
	double cs = 
		comparator.calculateContainmentSimilarity(
			NGramGraphCache.at(first_text_id),
			NGramGraphCache.at(second_text_id),
			"CS"
		);
	return 1 - cs;
	
}



DistMat* ngg_compute_distance_matrix(char** docs, int ndocs){	
	DistMat* DM = new_distance_matrix(ndocs);
	const int maxcap = MAX_MEMORY_GRAPHS/2;
	int nparts = std::ceil(double(ndocs)/maxcap);
	
	int ngraphs, offset, offset2;
	for(int p = 0; p < nparts ; p++){
		std::cout<<"Iteration " << p+1 << " / "<< nparts<<std::endl;
		offset = p*maxcap;
		if(p < nparts-1)  ngraphs = maxcap; else  ngraphs = ndocs-offset;
		ngg_compute_inner_partition_distances(DM, docs, ngraphs, offset);
		for(int cmp_p = p+1; cmp_p < nparts; cmp_p++){
			int cmp_offset = cmp_p*maxcap;
			if(cmp_p < nparts-1) ngraphs = maxcap; else ngraphs = ndocs-cmp_offset;
			ngg_compute_cross_partition_distances(DM, docs, ngraphs, offset, cmp_offset);
		}
		NGramGraphCache.clear();
	}
	return DM;
}


void ngg_compute_cross_partition_distances(DistMat* DM, char** docs, int ndocs, int offset1, int offset2){
	int nprev = NGramGraphCache.size();
	make_cache_graphs(docs+offset2, ndocs);
	for(int i = 0; i < nprev; i++){
		for(int j = 0 ; j < ndocs; j++){		
				DM->distances[i+offset1][j+offset2] =
				DM->distances[j+offset2][i+offset1]
				 	= ngg_dissimilarity(i,nprev+j);
				if((j+1)%MAX_MEMORY_GRAPHS/2==0){
					std::cout<<"cross D("<<i+offset1<<","<<j+offset2<<")"<<std::endl;
				}
		}
	}
	uncache_graphs(nprev,ndocs);
}


void ngg_compute_inner_partition_distances(DistMat* DM, char** docs, int ndocs, int offset){
	make_cache_graphs(docs+offset, ndocs);
	DistMat* PM =  ngg_compute_partition_distance_matrix(ndocs);
	for(int i = 0 ; i < ndocs; i++){
		for(int j = i ; j < ndocs; j++){
			DM->distances[i+offset][j+offset] = 
			DM->distances[j+offset][i+offset] =
				PM->distances[i][j];
			if((i+1)%MAX_MEMORY_GRAPHS/2==0)std::cout<<"inner D("<<i+offset<<","<<j+offset<<")"<<std::endl;
		}

	}
}



DistMat* ngg_compute_partition_distance_matrix(int nparts){
	DistMat* PM = new_distance_matrix(nparts);
	for(int i = 0; i < nparts; i++){
		for(int j = i; j < nparts; j++){
			PM->distances[i][j] =
			PM->distances[j][i] 
				= ngg_dissimilarity(i,j);
		}
	}
	return PM;
}

void mat_vis(DistMat* DM){
	for(int i = 0; i < DM->n; i++){
		for(int j = 0 ; j < DM->n; j++){	
			std::cout << DM->distances[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

#ifdef __cplusplus
}



#endif
