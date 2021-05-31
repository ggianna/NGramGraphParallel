#include "stdio.h"
#include "C_Interface.h"
#include "assert.h"

int main(void){
	printf("\tsending request to server..\n");
	char msg[20] = "Hello World";
	assert(msg);

	for(int i = 0 ; i < 3 ; i ++){
	
		int graphid = HandleRequest(msg);
		printf("graphid=%d\n",i);
	}
	printf("client terminating\n");
}
