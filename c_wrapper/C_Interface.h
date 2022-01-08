#ifndef C_INTERFACE_H
#define C_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

// static const char* objects_storage_file = "/home/ngialitsis/search/dmat/dmat_20news_raw_n5_w5_cs.bin";
// static const char* objects_storage_file = "/home/ngialitsis/search/dmat/dmat_20news_raw_n5_w5_mincs.bin";
// static const char* objects_storage_file = "/home/ngialitsis/search/dmat/dmat_20news_raw_n5_w5_sqrt_cs.bin";
static const char* objects_storage_file = "/home/ngialitsis/search/dmat/dmat_20news_raw_n5_w5_sqrt_mincs.bin";


typedef struct  DistMat{
    double** distances;
    int n;
}DistMat;

double ngg_dissimilarity(int first_text_id, int second_text_id);
void ngg_construct(int text_id, const char* text);
void make_cache_graphs(char** ptrs, int num_graphs);
DistMat* ngg_compute_partition_distance_matrix(int n_parts);
void ngg_visualize_distance_matrix(DistMat* DM, int n_parts);
void ngg_compute_inner_partition_distances(DistMat* DM, char** docs, int ndocs, int offset);
void ngg_compute_cross_partition_distances(DistMat* DM, char** docs, int ndocs, int offset1, int offset2);
DistMat* ngg_compute_distance_matrix(char** docs, int ndocs);
DistMat* new_square_mat(int n);
void mat_vis(DistMat* mat);
void cerealize(DistMat* DM, const char* binfile);
void decerialize(const char* binfile);
double get_precomputed_distance_if_exists(int first_text_id, int second_text_id);

#ifdef __cplusplus
}
#endif
#endif
