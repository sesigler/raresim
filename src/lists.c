#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <err.h>
#include <sysexits.h>
#include <string.h>
#include <inttypes.h>

#include "lists.h"

//{{{ uint32_t_array
//{{{ struct uint32_t_array *uint32_t_array_init(uint32_t init_size)
struct uint32_t_array *uint32_t_array_init(uint32_t init_size)
{
    struct uint32_t_array *ua = 
            (struct uint32_t_array *) malloc(sizeof(struct uint32_t_array));
    if (ua == NULL)
        err(1, "alloc error in uint32_t_array_init().\n");

    ua->size = init_size;
    ua->data = (uint32_t *)malloc(init_size * sizeof(uint32_t));
    if (ua->data == NULL)
        err(1, "alloc error in uint32_t_array_init().\n");

    ua->num = 0;
    return ua;
}
//}}}

//{{{void uint32_t_array_destroy(struct uint32_t_array **ua);
void uint32_t_array_destroy(struct uint32_t_array **ua)
{
    free((*ua)->data);
    free(*ua);
    *ua = NULL;
}
//}}}

//{{{uint32_t uint32_t_array_add(struct uint32_t_array *oi)
uint32_t uint32_t_array_add(struct uint32_t_array *ua, uint32_t val)
{
    if (ua->num == ua->size) {
        ua->size = ua->size * 2;
        ua->data = (uint32_t *)
                realloc(ua->data, ua->size * sizeof(uint32_t));

        if (ua->data == NULL)
            err(1, "alloc error in uint32_t_array_add().\n");

        memset(ua->data + ua->num, 0, (ua->size - ua->num) * sizeof(uint32_t));
    }

    ua->data[ua->num] = val;
    ua->num = ua->num + 1;
    return ua->num - 1;
}
//}}}

//{{{uint32_t uint32_t_array_set(
uint32_t uint32_t_array_set(struct uint32_t_array *ua,
                            uint32_t val,
                            uint32_t index)
{
    while (ua->size < index) {
        ua->size = ua->size * 2;
        ua->data = (uint32_t *)
                realloc(ua->data, ua->size * sizeof(uint32_t));
        if (ua->data == NULL)
            err(1, "alloc error in uint32_t_array_add().\n");
        memset(ua->data + ua->num, 0, (ua->size - ua->num) * sizeof(uint32_t));
    }

    ua->data[index] = val;
    if (index + 1 > ua->num)
        ua->num = index + 1;
    return index;
}
//}}}

//{{{uint32_t *uint32_t_array_get(struct uint32_t_array *ua, uint32_t index)
uint32_t *uint32_t_array_get(struct uint32_t_array *ua, uint32_t index)
{
    if (index > ua->size)
        return NULL;
    else
        return &(ua->data[index]);
}
//}}}

//{"{uint32_t uint32_t_array_write(struct uint32_t_array *ua, FILE *fp)
uint32_t uint32_t_array_write(struct uint32_t_array *ua, FILE *fp)
{
    if (fwrite(&(ua->num), sizeof(uint32_t), 1, fp) != 1)
        err(1, "Could not write uint32_t_array size");

    if (fwrite(ua->data, sizeof(uint32_t), ua->num, fp) != ua->num)
        err(1, "Could not write uint32_t_array data");

    return 1 + ua->num;
}
//}}}

//{{{struct uint32_t_array *uint32_t_array_read(FILE *fp)
struct uint32_t_array *uint32_t_array_read(FILE *fp)
{
    struct uint32_t_array *ua = 
            (struct uint32_t_array *) malloc(sizeof(struct uint32_t_array));
    if (ua == NULL)
        err(1, "alloc error in uint32_t_array_read().\n");

    uint32_t v;
    size_t fr = fread(&v, sizeof(uint32_t), 1, fp);
    ua->num = v;
    ua->size = v;

    ua->data = (uint32_t *)malloc(ua->num * sizeof(uint32_t));
    if (ua->data == NULL)
        err(1, "alloc error in uint32_t_array_read().\n");

    fr = fread(ua->data, sizeof(uint32_t), ua->num, fp);

    return ua;
}
///}}}

//}}}

//{{{ uint32_t_sparse_matrix
//
//{{{ struct uint32_t_array *uint32_t_sparse_matrix_init(uint32_t rows
struct uint32_t_sparse_matrix *uint32_t_sparse_matrix_init(uint32_t rows,
                                                           uint32_t cols)
{
    struct uint32_t_sparse_matrix *m = 
            (struct uint32_t_sparse_matrix *)
            malloc(sizeof(struct uint32_t_sparse_matrix));
    if (m == NULL)
        err(1, "malloc error in uint32_t_sparse_matrix_init().\n");

    m->size = rows;
    m->rows = 0;
    m->data =  (struct uint32_t_array **) 
        malloc(rows * sizeof(struct uint32_t_array *));

    if (m->data == NULL)
        err(1, "malloc error in uint32_t_sparse_matrix_init().\n");

    int i;
    for (i =0; i < rows; ++i) {
        m->data[i] = uint32_t_array_init(cols);

        if (m->data[i] == NULL)
            err(1, "alloc error in uint32_t_sparse_matrix_init().\n");
    }

    return m;
}
//}}}

//{{{ void uint32_t_sparse_matrix_destroy(struct uint32_t_sparse_matrix **ua);
void uint32_t_sparse_matrix_destroy(struct uint32_t_sparse_matrix **m)
{
    int i;
    for (i = 0; i < (*m)->size; ++i) {
        if ( (*m)->data[i] != NULL) {
            uint32_t_array_destroy(&((*m)->data[i]));
        }
    }

    free((*m)->data);
    free(*m);
    *m = NULL;
}
//}}}

//{{{uint32_t uint32_t_sparse_martix_add(struct uint32_t_sparse_matrix *m,
uint32_t uint32_t_sparse_matrix_add(struct uint32_t_sparse_matrix *m,
                                    uint32_t row,
                                    uint32_t val)
{
    while (m->size <= row + 1) {
        uint32_t old_size = m->size;
        m->size = m->size * 2;
        m->data = (struct uint32_t_array **)
                realloc(m->data, m->size * sizeof(struct uint32_t_array *));
        if (m->data == NULL)
            err(1, "alloc error in uint32_t_sparse_martix_add().\n");

        int i;
        for (i = old_size; i < m->size; ++i) {
            struct uint32_t_array *ua = uint32_t_array_init(10);
            m->data[i] = ua;

            if (m->data[i] == NULL)
                err(1, "alloc error in uint32_t_sparse_martix_add().\n");
        }
    }

    if (m->rows < row)
        m->rows = row + 1;

    uint32_t ret = uint32_t_array_add(m->data[row], val);

    return m->size;
}
//}}}

//{{{uint32_t *uint32_t_sparse_martix_get(struct uint32_t_sparse_matrix *m,
uint32_t *uint32_t_sparse_martix_get(struct uint32_t_sparse_matrix *m,
                                     uint32_t row,
                                     uint32_t col)
{
    if (row > m->rows)
        return NULL;
    else
        return uint32_t_array_get(m->data[row], col);
}
//}}}

//{{{uint32_t uint32_t_sparse_matrix_write(struct uint32_t_sparse_matrix *m,
uint32_t uint32_t_sparse_matrix_write(struct uint32_t_sparse_matrix *m,
                                       FILE *fp)
{
    uint32_t written = 0;

    if (fwrite(&(m->rows), sizeof(uint32_t), 1, fp) != 1)
        err(1, "Could not write uint32_t_sparse_matrix number of rows");

    written += 1;

    uint32_t *sizes = (uint32_t *)malloc(m->rows * sizeof(uint32_t));

    int i;
    uint32_t v = 0;
    for (i = 0; i < m->rows; ++i) {
        v += m->data[i]->num;
        sizes[i] = v;
    }

    if (fwrite(sizes, sizeof(uint32_t), m->rows, fp) != m->rows)
        err(1, "Could not write uint32_t_sparse_matrix rows sizes");

    written += m->rows;

    for (i = 0; i < m->rows; ++i) {
        if (m->data[i]->num > 0) {
            if (fwrite(m->data[i]->data,
                       sizeof(uint32_t),
                       m->data[i]->num, fp) != m->data[i]->num)
                err(1, "Could not write uint32_t_sparse_matrix row data");
            written += m->data[i]->num;
        }
    }

    return written;
}
//}}}

//{{struct uint32_t_sparse_matrix *uint32_t_sparse_matrix_read(FILE *fp);
struct uint32_t_sparse_matrix *uint32_t_sparse_matrix_read(FILE *fp)
{
    struct uint32_t_sparse_matrix *m = 
            (struct uint32_t_sparse_matrix *)
            malloc(sizeof(struct uint32_t_sparse_matrix));
    if (m == NULL)
        err(1, "malloc error in uint32_t_sparse_matrix_read().\n");

    uint32_t v;
    size_t fr = fread(&v, sizeof(uint32_t), 1, fp);
    m->size = v;
    m->rows = v;

    m->data =  (struct uint32_t_array **) 
        malloc(m->rows * sizeof(struct uint32_t_array *));


    uint32_t *sizes = (uint32_t *)malloc(m->rows * sizeof(uint32_t));
    fr = fread(sizes, sizeof(uint32_t), m->rows, fp);

    int i;
    uint32_t last_size = 0;
    for (i = 0; i < m->rows; ++i) {
        uint32_t curr_size = sizes[i] - last_size;

        if (curr_size == 0) {
            m->data[i] = NULL;
        } else {
            m->data[i] = 
                    (struct uint32_t_array *)
                    malloc(sizeof(struct uint32_t_array));
            if ( m->data[i] == NULL)
                err(1, "alloc error in uint32_t_sparse_matrix_read().\n");

            m->data[i]->size = curr_size;
            m->data[i]->data =
                    (uint32_t *)malloc(curr_size * sizeof(uint32_t));
            if (m->data[i]->data == NULL)
                err(1, "alloc error in uint32_t_sparse_matrix_read().\n");

            fr = fread(m->data[i]->data, sizeof(uint32_t), curr_size, fp);
            m->data[i]->num = curr_size;
        }

        last_size = sizes[i];
    }

    free(sizes);

    return m;
}
//}}}

//}}}
