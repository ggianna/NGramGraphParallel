#ifndef C_INTERFACE_H
#define C_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif
double computeDissimilarity(const char* msg1, const char* msg2);
double valueDissimilarity(const char* msg1,const char* msg2);
#ifdef __cplusplus
}
#endif
#endif
