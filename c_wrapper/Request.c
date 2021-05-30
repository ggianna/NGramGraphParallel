#include "stdio.h"
#include "C_Interface.hpp"
#include "assert.h"

int main(void){
	printf("\tsending request to server..\n");
	char msg[10] = "ngg";
	assert(msg);

	for(int i = 0 ; i < 3 ; i ++){
	
		int graphid = Request(msg);
		printf("graphid=%d\n",i);
	}
	printf("client terminating");
}
