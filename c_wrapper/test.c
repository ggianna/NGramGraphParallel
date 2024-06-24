#include "stdio.h"
#include "C_Interface.h"
#include "assert.h"
#include "stdlib.h"

int main(void){
	printf("\tsending request to server..\n");
    int ndocs = 3;
	char** docs = malloc(ndocs*sizeof(char*));
    for(int i = 0; i < ndocs; i++){
        docs[i] = (char *) malloc(100);
        scanf("%s", &docs[i]);
    }
    ngg_construct_graph_database();

}
