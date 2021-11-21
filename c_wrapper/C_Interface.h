#ifndef C_INTERFACE_H
#define C_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif
// double computeDissimilarity(const char* msg1, const char* msg2);
// double valueDissimilarity(const char* msg1,const char* msg2);
double ngg_dissimilarity(int first_text_id, int second_text_id);
void ngg_construct(int text_id, const char* text);
void make_cache_graphs(char** ptrs, int num_graphs);
double** ngg_compute_partition_distance_matrix(int n_parts);
void ngg_visualize_distance_matrix(double** DistMat, int n_parts);
void ngg_compute_inner_partition_distances(double** DistMat, char** docs, int ndocs, int offset);
void ngg_compute_cross_partition_distances(double** DistMat, char** docs, int ndocs, int offset1, int offset2);
double** ngg_compute_distance_matrix(char** docs, int ndocs);
double** new_square_mat(int n);
#ifdef __cplusplus
}
#endif
#endif
