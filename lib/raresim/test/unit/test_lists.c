#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <err.h>

#include "unity.h"
#include "lists.h"

void setUp(void) { }
void tearDown(void) { }

//{{{void test_sampling(void)
void test_sampling(void)
{
    uint32_t *R = (uint32_t *) malloc(10 * sizeof(uint32_t));
    reservoir_sample(100, 10, R);

    uint32_t i;
    for (i = 0; i < 10; i++) {
        TEST_ASSERT_TRUE(R[i] <= 100);
    }
    free(R);
}
//}}}

//{{{void test_uint32_t_array(void)
void test_uint32_t_array(void)
{
    /*
    struct uint32_t_array
    {
        uint32_t num, size, *data;
    };

    struct uint32_t_array *uint32_t_array_init(uint32_t init_size);
    void uint32_t_array_destroy(struct uint32_t_array **ua);
    uint32_t uint32_t_array_add(struct uint32_t_array *ua, uint32_t val);
    uint32_t *uint32_t_array_get(struct uint32_t_array *ua, uint32_t index);
    */

    struct uint32_t_array *ua = uint32_t_array_init(10);

    TEST_ASSERT_EQUAL(10, ua->size);
    TEST_ASSERT_EQUAL(0, ua->num);

    uint32_t ret = uint32_t_array_add(ua, 5);

    TEST_ASSERT_EQUAL(10, ua->size);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_EQUAL(1, ua->num);
    TEST_ASSERT_EQUAL(5, ua->data[0]);

    uint32_t *ret_p = uint32_t_array_get(ua, 0);
    TEST_ASSERT_EQUAL(5, *ret_p);

    ret_p = uint32_t_array_get(ua, 100);
    TEST_ASSERT_EQUAL(NULL, ret_p);


    uint32_t i;
    for (i = 0; i < 10; ++i) {
        ret = uint32_t_array_add(ua, 5);
        TEST_ASSERT_EQUAL(i + 1, ret);
    }

    TEST_ASSERT_EQUAL(20, ua->size);
    TEST_ASSERT_EQUAL(11, ua->num);
    char *file_name ="test_file.dat";


    ret = uint32_t_array_write(ua, file_name);

    TEST_ASSERT_EQUAL(1 + ua->num, ret);




    struct uint32_t_array *ub = uint32_t_array_read(file_name);
    TEST_ASSERT_EQUAL(ua->num, ub->num);


    for (i = 0; i < ua->num; ++i) {
        TEST_ASSERT_EQUAL(ua->data[i], ub->data[i]);
    }

    uint32_t_array_destroy(&ua);
    uint32_t_array_destroy(&ub);
}
//}}}

//{{{void test_uint32_t_sparse_matrix(void)
void test_uint32_t_sparse_matrix(void)
{
    struct uint32_t_sparse_matrix *m = uint32_t_sparse_matrix_init(10, 10);
    TEST_ASSERT_EQUAL(10, m->size);
    TEST_ASSERT_EQUAL(0, m->rows);

    uint32_t ret = uint32_t_sparse_matrix_add(m, 3, 10);

    TEST_ASSERT_EQUAL(10, m->data[3]->data[0]);
    ret = uint32_t_sparse_matrix_add(m, 20, 99);

    TEST_ASSERT_EQUAL(99, m->data[20]->data[0]);

    uint32_t *ret_p = uint32_t_sparse_martix_get(m, 3, 0);
    TEST_ASSERT_EQUAL(10, *ret_p);

    ret_p = uint32_t_sparse_martix_get(m, 20, 0);
    TEST_ASSERT_EQUAL(99, *ret_p);

    FILE *fp = fopen("test_matrix_file.dat", "wb");
    if (fp == NULL)
        err(1, "Could not open test_matrix_file.dat");

    ret = uint32_t_sparse_matrix_write(m, fp);
    fclose(fp);
    TEST_ASSERT_EQUAL(25, ret);
    char *filename = "test_matrix_file.dat";


    struct uint32_t_sparse_matrix *m1 = uint32_t_sparse_matrix_read(filename);
    TEST_ASSERT_EQUAL(m1->cols, m->cols);

    ret_p = uint32_t_sparse_martix_get(m1, 20, 0);
    TEST_ASSERT_EQUAL(99, *ret_p);


    ret = uint32_t_sparse_matrix_add(m, 10, 99);

    ret = uint32_t_sparse_matrix_add(m, 11, 11);

    ret = uint32_t_sparse_matrix_add(m, 12, 120);
    ret = uint32_t_sparse_matrix_add(m, 12, 121);

    ret = uint32_t_sparse_matrix_add(m, 13, 130);
    ret = uint32_t_sparse_matrix_add(m, 13, 131);
    ret = uint32_t_sparse_matrix_add(m, 13, 132);
    ret = uint32_t_sparse_matrix_add(m, 13, 133);
    ret = uint32_t_sparse_matrix_add(m, 13, 134);
    ret = uint32_t_sparse_matrix_add(m, 13, 135);
    ret = uint32_t_sparse_matrix_add(m, 13, 136);
    ret = uint32_t_sparse_matrix_add(m, 13, 137);
    ret = uint32_t_sparse_matrix_add(m, 13, 138);
    ret = uint32_t_sparse_matrix_add(m, 13, 139);

    ret_p = uint32_t_sparse_martix_get(m, 13, 3);
    TEST_ASSERT_EQUAL(133, *ret_p);

    uint32_t_sparse_martix_remove_row(m, 20);
    TEST_ASSERT_EQUAL(14, m->rows);

    uint32_t old_row_num = m->data[13]->num;
    ret = uint32_t_sparse_martix_prune_row(m, 13, 0);
    TEST_ASSERT_EQUAL(old_row_num, ret);

    ret = uint32_t_sparse_martix_prune_row(m, 13, 3);
    TEST_ASSERT_EQUAL(old_row_num - 3, ret);

    ret = uint32_t_sparse_martix_prune_row(m, 13, 3);
    TEST_ASSERT_EQUAL(old_row_num - 3 - 3, ret);

    uint32_t_sparse_matrix_destroy(&m);
    uint32_t_sparse_matrix_destroy(&m1);
}
//}}}

//{{{void test_add_buffer_to_matrix(void)
void test_add_buffer_to_matrix(void)
{
    struct uint32_t_sparse_matrix *m = uint32_t_sparse_matrix_init(10, 10);
    char *buffer = "1 1 0 0 1 1 0 0 1 1";
    uint32_t row = 0, col = 0;
    uint32_t max_col = add_buffer_to_matrix(buffer,
                                            strlen(buffer),
                                            m,
                                            &row,
                                            &col);
    TEST_ASSERT_EQUAL(max_col, 10);

    max_col = add_buffer_to_matrix(buffer,
                                   strlen(buffer),
                                   m,
                                   &row,
                                   &col);
    TEST_ASSERT_EQUAL(max_col, 20);


    uint32_t *ret_p = uint32_t_sparse_martix_get(m, 0, 0);
    TEST_ASSERT_EQUAL(*ret_p, 0);
    ret_p = uint32_t_sparse_martix_get(m, 0, 1);
    TEST_ASSERT_EQUAL(*ret_p, 1);
    ret_p = uint32_t_sparse_martix_get(m, 0, 2);
    TEST_ASSERT_EQUAL(*ret_p, 4);

    uint32_t_sparse_matrix_destroy(&m);
}
//}}}

//{{{void test_uint32_t_sparse_matrix_compress_read(void)
void test_uint32_t_sparse_matrix_compress_read(void)
{
    char *file_name = "../data/test.haps.gz";
    struct uint32_t_sparse_matrix *m1 = read_compressed_matrix(file_name);
    TEST_ASSERT_EQUAL(10, m1->cols);

    uint32_t exp_1[6] = {0,2,4,6,8,9};
    uint32_t i;
    uint32_t *ret_p;
    for (i=0; i < 6; ++i) {
        ret_p = uint32_t_sparse_martix_get(m1, 0, i);
        TEST_ASSERT_EQUAL(exp_1[i], *ret_p);
    }

    uint32_t exp_2[4] = {2,3,6,7};
    for (i=0; i < 4; ++i) {
        ret_p = uint32_t_sparse_martix_get(m1, 2, i);
        TEST_ASSERT_EQUAL(exp_2[i], *ret_p);
    }

    uint32_t_sparse_matrix_destroy(&m1);

    struct uint32_t_sparse_matrix *mu = read_matrix("../data/bigger_test.haps");
    TEST_ASSERT_EQUAL(55, mu->cols);

    struct uint32_t_sparse_matrix *mc =
            read_matrix("../data/bigger_test.haps.gz");
    TEST_ASSERT_EQUAL(55, mc->cols);


    TEST_ASSERT_EQUAL(mu->rows, mc->rows);

    uint32_t *ret_pc, *ret_pu;
    for (i=0; i < mu->rows; i++) {
        TEST_ASSERT_EQUAL(mu->data[i]->num, mc->data[i]->num); 
        uint32_t j;
        for (j=0; j < mu->data[i]->num; j++) {
            ret_pc = uint32_t_sparse_martix_get(mc, i, j);
            ret_pu = uint32_t_sparse_martix_get(mu, i, j);
            TEST_ASSERT_EQUAL(*ret_pc, *ret_pu);
        }

    }


    uint32_t_sparse_matrix_destroy(&mu);
    uint32_t_sparse_matrix_destroy(&mc);
}
//}}
