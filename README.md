# raresim
Python Interface for Scalable rare-variant simulations.


##To Install:  
  $ Clone git ..  <br/>
  $ cd Path to raresim/    <br/>
  $ python3 setup.py install <br/>

##Usage:

### Covert haplotype files to a sparse matrix

```
usage: convert.py [-h] -i INPUT_FILE -o OUTPUT_FILE

optional arguments:
  -h, --help      show this help message and exit
  -i INPUT_FILE   Input haplotype file path
  -o OUTPUT_FILE  Ouput sparse matrix path
```

```
$ python convert.py \
    -i lib/raresim/test/data/Simulated_80k_9.controls.haps.gz \
    -o Simulated_80k_9.controls.haps.gz.sm
```

### Simulate new allele frequencies

```
usage: sim.py [-h] -m SPARSE_MATRIX -b EXP_BINS -l INPUT_LEGEND -L
              OUTPUT_LEGEND -H OUTPUT_HAP

optional arguments:
 -h, --help        show this help message and exit
 -m SPARSE_MATRIX  Input sparse matrix path
 -b EXP_BINS       Input expected bin sizes
 -l INPUT_LEGEND   Input variant site legend
 -L OUTPUT_LEGEND  Output variant site legend
 -H OUTPUT_HAP     Output compress hap file
```

```
$ python sim.py \
    -m Simulated_80k_9.controls.haps.gz.sm \
    -b lib/raresim/test/data/Expected_variants_per_bin_80k.txt \
    -l lib/raresim/test/data/Simulated_80k.legend \
    -L new.legend \
    -H new.hap.gz

Input allele frequency distribution:
(1, 1, 20.0) 9
(2, 2, 10.0) 5
(3, 5, 5.0) 6
(6, 10, 5.0) 7
(11, 20, 1.0) 11
(21, 1000, 0.0) 48

New allele frequency distribution:
(1, 1, 20.0) 15
(2, 2, 10.0) 11
(3, 5, 5.0) 6
(6, 10, 5.0) 3
(11, 20, 1.0) 1
(21, 1000, 0.0) 0

Writing new variant legend

Writing new haplotype file............
```


Python code example: <br/>
```python
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
  ```


## Running C code

### Build

```
cd lib/raresim/src/
make
```

### Run

```
gunzip test/data/Simulated_80k_9.controls.haps.gz
./read \
    -i ../test/data/Simulated_80k_9.controls.haps \
    -o Simulated_80k_9.controls.haps.dat \
```
