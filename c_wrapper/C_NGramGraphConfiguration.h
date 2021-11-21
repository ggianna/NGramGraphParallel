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


typedef class SerializableRow{
  public: 
      double row;
      SerializableRow(double x){
        this->row = x;
      };
      SerializableRow(){};
      ~SerializableRow(){};
      template <class Archive>
      void save( Archive & ar ) const
      {
        std::cout<<"serialize"<<std::endl;
        ar( row );
      }
          
      template <class Archive>
      void load( Archive & ar )
      {
        std::cout<<"deserialize"<<std::endl;
        ar( row );
      }

}SerializableRow;

// class SerializableDistMat{
//     public:
        
//         std::shared_ptr<SerializableRow> data;
//         template <class Archive>
//         void save( Archive & ar ) const{
//             ar(data);
//         }
                
//         template <class Archive>
//         void load( Archive & ar ){
//             ar(data);
//         }
// };


static const int NGRAMSIZE_VALUE = 1;
static const int WINDOWSIZE = 1;
static const int MAX_MEMORY_GRAPHS = 4;
// static ProximityApproach* approach = new SymmetricApproach();
static ProximityApproach* approach = new NonSymmetricApproach();
static GraphComparator<std::string, std::string> comparator;
static StringSplitter stringSplitter(NGRAMSIZE_VALUE);
static std::vector<NGramGraph> NGramGraphCache;


#endif

