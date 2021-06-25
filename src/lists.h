#ifndef __LISTS_H__
#define __LISTS_H__

#include <stdint.h>

double rand_double();

// UINT32 ARRAY
struct uint32_t_array
{
    uint32_t num, size, *data;
};

struct uint32_t_array *uint32_t_array_init(uint32_t init_size);
void uint32_t_array_destroy(struct uint32_t_array **ua);
uint32_t uint32_t_array_add(struct uint32_t_array *ua, uint32_t val);
uint32_t uint32_t_array_set(struct uint32_t_array *ua,
                            uint32_t val,
                            uint32_t index);
uint32_t *uint32_t_array_get(struct uint32_t_array *ua, uint32_t index);
uint32_t uint32_t_array_write(struct uint32_t_array *ua, FILE *fp);
struct uint32_t_array *uint32_t_array_read(FILE *fp);

// UINT32 SPARSE MATRIX
struct uint32_t_sparse_matrix
{
    uint32_t rows, size;
    struct uint32_t_array **data;

};

struct uint32_t_sparse_matrix *uint32_t_sparse_matrix_init(uint32_t rows,
                                                           uint32_t cols);

void uint32_t_sparse_matrix_destroy(struct uint32_t_sparse_matrix **m);

uint32_t uint32_t_sparse_matrix_add(struct uint32_t_sparse_matrix *m,
                                    uint32_t row,
                                    uint32_t val);

uint32_t *uint32_t_sparse_martix_get(struct uint32_t_sparse_matrix *m,
                                     uint32_t row,
                                     uint32_t col);
uint32_t uint32_t_sparse_matrix_write(struct uint32_t_sparse_matrix *m,
                                      FILE *fp);
struct uint32_t_sparse_matrix *uint32_t_sparse_matrix_read(FILE *fp);

void uint32_t_sparse_martix_remove_row(struct uint32_t_sparse_matrix *m,
                                       uint32_t row);

uint32_t uint32_t_sparse_martix_prune_row(struct uint32_t_sparse_matrix *m,
                                          uint32_t row,
                                          double p_of_rem);

struct uint32_t_sparse_matrix *read_matrix(char *file_name);

void write_matrix(struct uint32_t_sparse_matrix *m, char *file_name);
#endif
