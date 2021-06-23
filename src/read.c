#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "lists.h"

#define SPACE 32
#define NEWLINE 10
#define ZERO 48
#define ONE 49


struct uint32_t_sparse_matrix *get_matrix(char *file_name)
{
    char *buffer;
    long length = 0;
    FILE *f = fopen(file_name, "rb");

    struct uint32_t_sparse_matrix *M = uint32_t_sparse_matrix_init(10, 10);

    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        length = ftell (f);
        fseek(f, 0, SEEK_SET);
        buffer = (char*) malloc ((length+1)*sizeof(char));
        if (buffer) {
            fread(buffer, sizeof(char), length, f);
        }
        fclose(f);
    }

    int col = 0, row = 0;

    long i;
    for (i = 0; i < length; i++) {

        if ( (int)buffer[i] == SPACE  )
            continue;

        if ( (int)buffer[i] == NEWLINE ) {
            col = 0;
            row += 1;
        }

        if ( (int)buffer[i] == ONE ) {
            int r = uint32_t_sparse_matrix_add(M, row, col);
        }

        col += 1;
    }

    return M;
}

int main(int argc, char **argv)
{
    struct uint32_t_sparse_matrix *m = get_matrix(argv[1]);
    printf("%d\n", m->rows);
 
    FILE *fp = fopen(argv[2], "wb");
    if (fp == NULL)
        err(1, "Could not open %s", argv[2]);

    uint32_t ret = uint32_t_sparse_matrix_write(m, fp);
    fclose(fp);

}
