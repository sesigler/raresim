#include <stdio.h>
#include <stdlib.h>


//Following libraries are broken on winddows:
#include <sysexits.h>
#include <err.h>
#include <getopt.h>


#include <ctype.h>
#include <zlib.h>

#include "lists.h"

int help(int exit_code)
{
    fprintf(stderr,
            "usage:   convert -i <input file path> -o <output file path>\n");
    return exit_code;
}

int main(int argc, char **argv)
{

    int c;
    char *input_file_name = NULL;
    char *output_file_name = NULL;

    while((c = getopt (argc, argv, "i:o:")) != -1) {
        switch(c) {
            case 'i':
                input_file_name = optarg;
                break;
            case 'o':
                output_file_name = optarg;
                break;
            case 'h':
                return help(EX_OK);
            case '?':
                if ( (optopt == 'i') ||
                     (optopt == 'o') )
                    fprintf (stderr,
                             "Option -%c requires an argument.\n",
                             optopt);
                else if (isprint (optopt))
                    fprintf (stderr,
                             "Unknown option `-%c'.\n",
                             optopt);
                else
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                return help(EX_USAGE);
            default:
                return help(EX_OK);
        }
    }

    if (input_file_name == NULL) {
        fprintf(stderr, "Input file required\n");
        return help(EX_USAGE);
    } else if (output_file_name == NULL) {
        fprintf(stderr, "Output file required\n");
        return help(EX_USAGE);
    }

    struct uint32_t_sparse_matrix *m = read_matrix(input_file_name);
    printf("%d\n", m->rows);
    write_matrix(m, output_file_name);
    uint32_t_sparse_matrix_destroy(&m);
}
