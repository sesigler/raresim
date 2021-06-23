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
