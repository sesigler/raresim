from rareSim import sparse
import gzip
import sys
from header import *

# could be slow with 100+ bins, optimizing with binary search would help


def main():
    args = get_args()

    func_split = get_split(args)

    bins = None
    if func_split:
        bins = {}
        bins['fun'] = read_expected(args.exp_fun_bins)
        bins['syn'] = read_expected(args.exp_syn_bins)
    else:
        bins = read_expected(args.exp_bins)

    legend_header, legend = read_legend(args.input_legend)

    M = sparse(None)
    M.load(args.sparse_matrix)

    verify_legend(legend, legend_header, M, func_split)

    bin_h = assign_bins(M, bins, legend, func_split)

    print('Input allele frequency distribution:')

    if func_split:
        print('Functional')
        print_bin(bin_h['fun'], bins['fun'])
        print('\nSynonymous')
        print_bin(bin_h['syn'], bins['syn'])
    else:
        print_bin(bin_h, bins)

    R = []

    print()
    print('New allele frequency distribution:')


    if func_split:
        R = {'fun':[], 'syn':[]}
        prune_bins(bin_h['fun'], bins['fun'], R['fun'], M)
        prune_bins(bin_h['syn'], bins['syn'], R['syn'], M)
    else:
        prune_bins(bin_h, bins, R, M)
    
    all_kept_rows = []

    if func_split:
        print('Functional')
        print_bin(bin_h['fun'], bins['fun'])

        for bin_id in range(len(bin_h['fun'])):
            all_kept_rows += bin_h['fun'][bin_id]

        print('\nSynonymous')
        print_bin(bin_h['syn'], bins['syn'])

        for bin_id in range(len(bin_h['syn'])):
            all_kept_rows += bin_h['syn'][bin_id]
    else:
        print_bin(bin_h, bins)

        for bin_id in range(len(bin_h)):
            all_kept_rows += bin_h[bin_id]

    all_kept_rows.sort()

    print()
    print('Writing new variant legend')
    write_legend(all_kept_rows, args.input_legend, args.output_legend)    

    print()
    print('Writing new haplotype file', end='', flush=True)
    write_hap(all_kept_rows, args.output_hap, M)

if __name__ == '__main__': main()