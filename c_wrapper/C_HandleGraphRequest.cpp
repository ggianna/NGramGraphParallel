#include "C_HandleGraphRequest.h"
#include <stdio.h>
#include <iostream>
#include <functional>
using namespace std;


std::function<double (const char*,const char*)> similarityFunction;
Handler::Handler( double(*similarityFunction)(const char*, const char*),  int ngram_size,  int window_size){
	extern std::function<double (const char*,const char*)> similarityFunction;
	this->NGRAMSIZE_VALUE = ngram_size;
	this->WINDOWSIZE = window_size;
	similarityFunction = similarityFunction;
}

double Handler::dissimilarity(const char* msg1, const char* msg2){
	extern std::function<double (const char*,const char*)> similarityFunction;
	return 1-similarityFunction(msg1, msg2);
}
