from rareSim import sparse 
import random
import sys
import argparse


def get_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('-i',
                        dest='input_file',
                        required=True,
                        help='Input Haplotype file')

    parser.add_argument('-o',
                        dest='output_file',
                        required=True,
                        help='Ouput cases path')
    
    parser.add_argument('--seed',
                        dest='seed',
                        type=int,
                        help='Seed for random sample')

    parser.add_argument('-n',
                        dest='num',
                        type=int,
                        help='Number of haplotypes to extract')

    args = parser.parse_args()

    return args

def main():
    args = get_args()
    random.seed(args.seed)
    with open(args.input_file) as f:
        line = f.readline()
        columns = line.split()
    size = len(columns)
    columnsToExtract = random.sample(range(0, size), args.num)
    columnsToExtract.sort()
    with open(args.output_file, 'w') as f:
        for l in open(args.input_file):
            cols = l.split()

            line = [cols[i] for i in columnsToExtract]
            f.write(" ".join(line) + "\n")

if __name__ == '__main__': main()
