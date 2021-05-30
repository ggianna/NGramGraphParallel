#include "C_Interface.hpp"
#include "C_HandleGraphRequest.hpp"
#ifdef __cplusplus
extern "C"{
#endif
int id=0;
int Request(const char* msg){
	extern int id;
	if (msg){
		getGraph(msg);
		id++;
		return id;
	}
}
#ifdef __cplusplus
}
#endif
