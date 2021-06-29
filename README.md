# raresim
Scalable rare-variant simulations

### Tests

```
gunzip test/data/Simulated_80k_9.controls.haps.gz
cd test/unit/
make
```

### Build

```
cd src/
make
```

### Run

```
gunzip test/data/Simulated_80k_9.controls.haps.gz
./read \
    ../test/data/Simulated_80k_9.controls.haps \
    Simulated_80k_9.controls.haps.dat \
```

### Python example

```
import random
from raresime impore Raresim
M = Raresim('Simulated_80k_9.controls.haps')

for row in M:
    ac = M[row].num
    p = PDF[ac]
    flip = random.uniform(0, 1)
    if flip < p:
        M.remove(row)

M.write('Simulated_80k_9.controls.haps.mod')
```
