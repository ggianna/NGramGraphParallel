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

typedef struct sPalDB
   { char *pals;  /* words all together */
     char **ptrs;  /* pointers to each word */
     int npals;	  /* number of words (there is space for 1 more) */
     int csize;   /* allocated c size */
     int *c;
   } PalDB;
static PalDB DB;



static const int NGRAMSIZE_VALUE = 5;
static const int WINDOWSIZE = 5;
static const int MAX_MEMORY_GRAPHS = 2;
// static ProximityApproach* approach = new SymmetricApproach();
static ProximityApproach* approach = new NonSymmetricApproach();
static GraphComparator<std::string, std::string> comparator;
static StringSplitter stringSplitter(NGRAMSIZE_VALUE);
static std::vector<NGramGraph> NGramGraphCache;

#endif

