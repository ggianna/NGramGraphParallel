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
    ngg_construct_graph_database(docs, ndocs);
    double dissimilarity = ngg_dissimilarity(0,1);
    assert(dissimilarity > 0);
}

void test_serial(char* text){
     ngg_serialize(text);
}

void test_distance_matrix(int n){
	char** docs = malloc((n+1)*sizeof(char*));
    docs[0] = NULL;
    for(int i = 1; i < n; i++){
        printf("\ttype sentence..\n");
        docs[i] = (char *) malloc(100);
        gets(docs[i]);
        printf("\t\t%s\n",docs[i]);
    }
    // ngg_construct_graph_database(docs, n);
    
    ngg_compute_distance_matrix(docs, n);
}

int main(void){
	test_distance_matrix(8);
}
