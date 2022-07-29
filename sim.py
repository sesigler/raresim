from rareSim import sparse
import gzip
import sys
from header import *


def main():
    args = get_args()
    legend_header, legend = read_legend(args.input_legend)
    try:
        func_split, fun_only, syn_only = get_split(args)
    except Exception as e:
        sys.exit(str(e))

    func_split,fun_only,syn_only = get_split(args)
    M = sparse(None)
    M.load(args.sparse_matrix)

    try:
        verify_legend(legend, legend_header, M, func_split, args.prob)
    except DifferingLengths as e:
        sys.exit(str(e))
    except MissingColumn as e:
        sys.exit(str(e))
    except MissingProbs as e:
        sys.exit(str(e))

    if args.prob:
        all_rows = []
        for row in range(M.num_rows()):
            all_rows.append(row)
        
        print('Writing new haplotype file', end='', flush=True)
        probSim(args, all_rows, legend, M)

    else:

        if args.input_legend is None or args.output_legend is None:
            sys.exit("Legend files not provided")

        bins = get_expected_bins(args, func_split, fun_only, syn_only)

        bin_h = assign_bins(M, bins, legend, func_split, fun_only, syn_only, args.z)
        print('Input allele frequency distribution:')
        print_frequency_distribution(bins, bin_h, func_split, fun_only, syn_only)
        R = []

        try:
            if func_split:
                R = {'fun':[], 'syn':[]}
                prune_bins(bin_h['fun'], bins['fun'], R['fun'], M)
                prune_bins(bin_h['syn'], bins['syn'], R['syn'], M)
            elif fun_only:
                prune_bins(bin_h['fun'], bins, R, M)
            elif syn_only:
                prune_bins(bin_h['syn'], bins, R, M)
            else:
                prune_bins(bin_h, bins, R, M)
        except Exception as e:
            sys.exit(str(e))

        print()
        print('New allele frequency distribution:')
        print_frequency_distribution(bins, bin_h, func_split, fun_only, syn_only)

        all_kept_rows = get_all_kept_rows(bin_h, R, func_split, fun_only, syn_only, args.z)
        
        print()
        print('Writing new variant legend')
        write_legend(all_kept_rows, args.input_legend, args.output_legend)    

        print()
        print('Writing new haplotype file', end='', flush=True)
        write_hap(all_kept_rows, args.output_hap, M)

if __name__ == '__main__': main()