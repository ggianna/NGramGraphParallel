#include <cstdlib>
#include "C_Interface.h"
#include "C_HandleGraphRequest.h"


#ifdef __cplusplus
extern "C"{
#endif
int id=0;

static Handler* handler=NULL;
int HandleRequest(const char* msg){
	extern int id;
	if (msg){
		handler = new Handler();
		handler->getGraph(msg);
		id++;
		return id;
	}
}
#ifdef __cplusplus
}
#endif
