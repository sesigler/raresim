from rareSim import sparse 
import random
import argparse
import gzip

# could be slow with 100+ bins, optimizing with binary search would help
def get_bin(bins, val):
    for i in range(len(bins)):
        if val >= bins[i][0] and val <= bins[i][1]:
            return i
    return i+1

def get_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('-m',
                        dest='sparse_matrix',
                        required=True,
                        help='Input sparse matrix path')

    parser.add_argument('-b',
                        dest='exp_bins',
                        required=True,
                        help='Input expected bin sizes')

    parser.add_argument('-l',
                        dest='input_legend',
                        required=True,
                        help='Input variant site legend')

    parser.add_argument('-L',
                        dest='output_legend',
                        required=True,
                        help='Output variant site legend')

    parser.add_argument('-H',
                        dest='output_hap',
                        required=True,
                        help='Output compress hap file')

    args = parser.parse_args()

    return args

def main():
    args = get_args()

    bins = []

    header = None
    for l in open(args.exp_bins):
        A = l.rstrip().split()

        if header == None:
            header = A
        else:
            bins.append( (int(A[0]), int(A[1]), float(A[2]) ) )

    M = sparse(None)
    M.load(args.sparse_matrix)

    bin_h = {}
    row_i = 0
    for row in range( M.num_rows()):
        row_num = M.row_num(row)

        if row_num > 0:

            bin_id = get_bin(bins, row_num)

            if bin_id not in bin_h:
                bin_h[bin_id] = []

            bin_h[bin_id].append(row_i)

        row_i += 1

    print('Input allele frequency distribution:')
    for bin_id in range(len(bin_h)):
        if bin_id < len(bins):
            print('[' + str(bins[bin_id][0]) + ',' \
            	  + str(bins[bin_id][1]) + ']\t' \
            	  + str(bins[bin_id][2]) + '\t' \
		  + str(len(bin_h[bin_id])))
        else:
            print('[' + str(bins[bin_id-1][1]+1) + ', ]\t' \
            	  +  '\t' \
		  + str(len(bin_h[bin_id])))

    R = []

    print()
    print('New allele frequency distribution:')

    for bin_id in reversed(range(len(bin_h))):

	# The last binn contains those variants with ACs 
	# greater than the bin size, and we keep all of them
        if bin_id == len(bins):
            continue

        need = bins[bin_id][2]
        have = len(bin_h[bin_id])

        if have > need:
            p_rem = 1 - float(need)/float(have)
            row_ids_to_rem = []
            for i in range(have):
                flip = random.uniform(0, 1)
                if flip <= p_rem:
                    row_ids_to_rem.append(bin_h[bin_id][i])
            for row_id in row_ids_to_rem:
                R.append(row_id)
                bin_h[bin_id].remove(row_id)
        elif have < need:
            p_add = float(need - have)/float(len(R))
            row_ids_to_add = []
            for i in range(len(R)):
                flip = random.uniform(0, 1)
                if flip <= p_add:
                    row_ids_to_add.append(R[i])
            for row_id in row_ids_to_add:
                num_to_keep = int(random.uniform(bins[bin_id][0],\
                                                 bins[bin_id][1]))
                num_to_rem = M.row_num(row_id) - num_to_keep
                left = M.prune_row(row_id, num_to_rem)
                assert  num_to_keep == left

                bin_h[bin_id].append(row_id)
                R.remove(row_id)

    all_kept_rows = []
#    for bin_id in range(len(bin_h)):
#        if bin_id < len(bins):
#            print(bins[bin_id], len(bin_h[bin_id]))
#        else:
#            print(len(bin_h[bin_id]))
#        all_kept_rows += bin_h[bin_id]

    for bin_id in range(len(bin_h)):
        if bin_id < len(bins):
            print('[' + str(bins[bin_id][0]) + ',' \
            	  + str(bins[bin_id][1]) + ']\t' \
            	  + str(bins[bin_id][2]) + '\t' \
		  + str(len(bin_h[bin_id])))
        else:
            print('[' + str(bins[bin_id-1][1]+1) + ', ]\t' \
            	  +  '\t' \
		  + str(len(bin_h[bin_id])))
        all_kept_rows += bin_h[bin_id]



    all_kept_rows.sort()

    header = None
    file_i = 0
    row_i = 0
    f = open(args.output_legend, 'w')

    print()
    print('Writing new variant legend')
    for l in open(args.input_legend):
        if row_i == len(all_kept_rows):
            break

        if header == None:
            f.write(l)
            header = True
        else:
            if file_i == all_kept_rows[row_i]:
                f.write(l)
                row_i+=1
            file_i+=1

    print()
    print('Writing new haplotype file', end='', flush=True)

    num_kept_rows = len(all_kept_rows)
    step = int(num_kept_rows/10)
    i = 0

    with gzip.open(args.output_hap, 'wb') as f:
        for row_i in all_kept_rows:
            row = []
            for col_i in range(M.row_num(row_i)):
                row.append(M.get(row_i, col_i))

            O = ['0'] * M.num_cols()

            for col_i in row:
                O[col_i] = '1'

            s = ' '.join(O) + '\n'
            f.write(s.encode())

            if (i % step == 0):
                print('.', end='', flush=True)


            i+=1
    print()

if __name__ == '__main__': main()
