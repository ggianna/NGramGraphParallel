#include "stdio.h"
#include "C_Interface.h"
#include "assert.h"
#include "stdlib.h"
#include <string.h>

void test_serial(char* text){
     ngg_serialize(text);   
}

void test_distance_matrix(int n){
	char** docs = malloc(n*sizeof(char*));
    for(int i = 0; i < n; i++){
        printf("\ttype sentence..\n");
        docs[i] = (char *) malloc(100);
        fgets(docs[i], 100, stdin) ;
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
