#ifndef C_INTERFACE_H
#define C_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif
// double computeDissimilarity(const char* msg1, const char* msg2);
// double valueDissimilarity(const char* msg1,const char* msg2);
double ngg_dissimilarity(int first_text_id, int second_text_id);
void ngg_construct(int text_id, const char* text);
void ngg_construct_graph_database(char** ptrs, int num_graphs);
void ngg_store_graph_database(const char* filename);
void ngg_load_graph_database(const char* filename);
#ifdef __cplusplus
}
#endif
#endif
