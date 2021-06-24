#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "lists.h"

int main(int argc, char **argv)
{
    struct uint32_t_sparse_matrix *m = get_matrix(argv[1]);
    printf("%d\n", m->rows);

 
    uint32_t i, total = 0, removed = 0, pruned = 0;
    for (i = 0; i < m->rows; i++) {
        uint32_t size = 0;
        if ( m->data[i] != NULL) {
            size = m->data[i]->num;

            double flip = rand_double();
            if (flip <= 0.3) { //remove row
                uint32_t_sparse_martix_remove_row(m, i);
                removed += 1;
            } else if (flip <=0.6) { // prune row
                uint32_t_sparse_martix_prune_row(m, i, 0.5);
                pruned += 1;
            }
        } 
        total += 1;
    }

    printf("total rows:%d pruned:%d removed:%d\n",
            total,
            pruned,
            removed);

    FILE *fp = fopen(argv[2], "wb");
    if (fp == NULL)
        err(1, "Could not open %s", argv[2]);

    uint32_t ret = uint32_t_sparse_matrix_write(m, fp);
    fclose(fp);
}
