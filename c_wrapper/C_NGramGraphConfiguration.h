#ifndef C_NGRAM_GRAPH_CONFIGURATION_H
#define C_NGRAM_GRAPH_CONFIGURATION_H
#include "C_HandleGraphRequest.h"
#include "GraphComparator.hpp"
#include "GraphSimilarity.hpp"
#include "GraphComparator.hpp"
#include "NGramGraph.hpp"
#include "StringSplitter.hpp"
#include "ProximityApproach.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/binary.hpp"

#include <iostream>
#include <fstream>

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

class SerializableDistMat{
    public:
        
        std::vector<std::vector<double>> distances;
        int n;
        SerializableDistMat(DistMat* DM){

          this->n = DM->n;

          distances.empty();
          distances.reserve(DM->n);
          for(int i = 0 ; i < DM->n ; i++){
            std::vector<double> row;
            row.reserve(DM->n);
            for(int j = 0 ; j < DM->n ; j++){
              row.push_back(DM->distances[i][j]);   
            }
            distances.push_back(row);
          }
        }
        SerializableDistMat(){;}
        template <class Archive>
        void save( Archive & ar ) const{
            ar(distances, n);
        }
                
        template <class Archive>
        void load( Archive & ar ){
            ar(distances, n);
        }
};



DistMat* new_distance_matrix(int n){
	DistMat* DM = new DistMat();
	DM->distances = new double*[n];
	for(int i = 0; i < n; i++){
		DM->distances[i] = new double[n];
		for(int j = 0; j < n; j++){
			DM->distances[i][j] = 0;
		}
	}
	DM->n = n;
	return DM;
}


static const int NGRAMSIZE_VALUE = 3;
static const int WINDOWSIZE = 3;
static const int MAX_MEMORY_GRAPHS = 2000;
// static ProximityApproach* approach = new SymmetricApproach();
static ProximityApproach* approach = new NonSymmetricApproach();
static GraphComparator<std::string, std::string> comparator;
static StringSplitter stringSplitter(NGRAMSIZE_VALUE);
static std::vector<NGramGraph> NGramGraphCache;
DistMat* new_distance_matrix(int n);

#endif

