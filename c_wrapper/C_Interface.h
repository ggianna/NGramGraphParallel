#ifndef C_INTERFACE_H
#define C_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

// typedef struct sPalDB
//    { char *pals;  /* words all together */
//      char **ptrs;  /* pointers to each word */
//      int npals;	  /* number of words (there is space for 1 more) */
//      int csize;   /* allocated c size */
//      int *c;
//    } PalDB;
// static PalDB DB;


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
void cerealize(DistMat* mat);


#ifdef __cplusplus
}
#endif
#endif
