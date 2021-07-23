from libcpp cimport *
from libc.stdio cimport *
from libc.stdint cimport uintptr_t, uint32_t

#include <Python/Python.h>


cdef extern from "lib/raresim/src/lists.h":

    cdef packed struct uint32_t_array:
        uint32_t num, size, *data

    cdef struct uint32_t_array_init:
        uint32_t init_size

    #uint32_t_array *uint32_t_array_init(uint32_t init_size)
    void uint32_t_array_destroy( uint32_t_array **ua)

    uint32_t uint32_t_array_add(uint32_t_array *ua, uint32_t val)
    uint32_t uint32_t_array_set(uint32_t_array *ua,uint32_t val, uint32_t index)
    uintptr_t uint32_t_array_get(uint32_t_array *ua, uint32_t index)
    uint32_t uint32_t_array_write(uint32_t_array *ua, char *file_name)
    uint32_t_array *uint32_t_array_read(char *file_name)

    #// UINT32 SPARSE MATRIX |||

    cdef struct uint32_t_sparse_matrix:
        uint32_t rows, size
        uint32_t_array **data

    uint32_t_sparse_matrix *uint32_t_sparse_matrix_init(uint32_t rows,
                                                        uint32_t cols)

    void uint32_t_sparse_matrix_destroy(uint32_t_sparse_matrix **m)

    uint32_t uint32_t_sparse_matrix_add(uint32_t_sparse_matrix *m,
                                        uint32_t row,
                                        uint32_t val)

    uintptr_t uint32_t_sparse_martix_get(uint32_t_sparse_matrix *m,
                                         uint32_t row,
                                         uint32_t col)

    uint32_t sparse_martix_get(uint32_t_sparse_matrix *m,
                                uint32_t row,
                                uint32_t col)

    uint32_t_sparse_matrix *uint32_t_sparse_matrix_read(char *file_name)

    void uint32_t_sparse_martix_remove_row(uint32_t_sparse_matrix *m,
                                           uint32_t row)

    uint32_t uint32_t_sparse_martix_num_rows(uint32_t_sparse_matrix *m)

    uint32_t uint32_t_sparse_martix_num_cols(uint32_t_sparse_matrix *m)

    uint32_t uint32_t_sparse_martix_not_Null(uint32_t_sparse_matrix *m,
                                             uint32_t row)

    uint32_t uint32_t_sparse_martix_row_num(uint32_t_sparse_matrix *m,
                                            uint32_t row)

    uint32_t uint32_t_sparse_martix_prune_row(uint32_t_sparse_matrix *m,
                                              uint32_t row,
                                              uint32_t num_prune)

    uint32_t_sparse_matrix *read_matrix(char *file_name)

    void write_matrix(uint32_t_sparse_matrix *m, char *file_name)
