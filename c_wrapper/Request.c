#include "stdio.h"
#include "C_Interface.h"
#include "assert.h"
#include "stdlib.h"
#include <string.h>

int main(void){
	
    int ndocs = 2;
	char** docs = malloc(ndocs*sizeof(char*));
    for(int i = 0; i < ndocs; i++){
        printf("\ttype sentence..\n");
        docs[i] = (char *) malloc(100);
        gets(docs[i]);
        printf("\t\t%s\n",docs[i]);
    }
    ngg_construct_graph_database(docs, ndocs);
    // ngg_store_graph_database('mydb.txt');
    // ngg_load_graph_database('mydb.txt');
}
