#include "stdio.h"
#include "C_Interface.h"
#include "assert.h"
#include "stdlib.h"
#include <string.h>

int main(void){
	
    int ndocs = 3;
	char** docs = malloc(ndocs*sizeof(char*));
    for(int i = 0; i < ndocs; i++){
        printf("\ttype sentence..\n");
        docs[i] = (char *) malloc(100);
        gets(docs[i]);
        printf("\t\t%s\n",docs[i]);
    }
    ngg_construct_graph_database(docs, ndocs);
    printf("%.2f\n",ngg_dissimilarity(0,0));
    printf("%.2f\n",ngg_dissimilarity(1,1));
    printf("%.2f\n",ngg_dissimilarity(2,2));
    printf("%.2f\n",ngg_dissimilarity(0,1));
    printf("%.2f\n",ngg_dissimilarity(0,2));
}
