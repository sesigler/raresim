from rareSim import sparse 

def main():
    hap = "lib/raresim/test/data/Simulated_80k_9.controls.haps.gz"
    M = sparse(hap)
    print(M.num_rows(), M.num_cols())
    for row in range( M.num_rows()):
        alts = []
        for i in range(M.row_num(row)):
            alts.append(M.get(row,i))
        print(row, alts)

    M.write('out.haps.dat')

if __name__ == '__main__': main()
