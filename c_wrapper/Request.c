#include "stdio.h"
#include "C_Interface.h"
#include "assert.h"
#include "stdlib.h"
#include <string.h>



void test_graph_construct(){
    int ndocs = 2;
	char** docs = malloc(ndocs*sizeof(char*));
    strcpy(docs, "hello world");
    strcpy(docs+1, "hello there");
    make_cache_graphs(docs, ndocs);
    double dissimilarity = ngg_dissimilarity(0,1);
    assert(dissimilarity > 0);
}

void test_serial(char* text){
     ngg_serialize(text);   
}

void test_distance_matrix(int n){
	char** docs = malloc(n*sizeof(char*));
    for(int i = 0; i < n; i++){
        printf("\ttype sentence..\n");
        docs[i] = (char *) malloc(100);
        gets(docs[i]);
        printf("\t\t%s\n",docs[i]);
    }
    // ngg_construct_graph_database(docs, n);
    
    DistMat* DM = ngg_compute_distance_matrix(docs, n);
    mat_vis(DM);
    cerealize(DM);
}

int main(void){
	test_distance_matrix(6);
}
