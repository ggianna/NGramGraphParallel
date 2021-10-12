#include "stdio.h"
#include "C_Interface.h"
#include "assert.h"

int main(void){
	printf("\tsending request to server..\n");
	char msg1[100];
	char msg2[100];
	while(1){
		memset(msg1,0,100);
		memset(msg2,0,100);
		scanf("%s", &msg1);
		scanf("%s", &msg2);
		printf("\n%s\n\t%s\nVS = %.2f\n",msg1,msg2,valueDissimilarity(msg1, msg2));
	}
}
