# raresim
Scalable rare-variant simulations


To Install:
  $ Clone git ..
  $ cd Path to raresim/
  $ python3 setup.py build_ext -i

```python
Pythpon example:
from rareSim import sparse 
import random
inps = "lib/raresim/test/data/Simulated_80k_9.controls.haps"

if __name__ == '__main__':
	M = sparse(inps)
	for row in range( M.rcount()):
		if (M.row_not_null(row)==1):
			flip = random.uniform(0, 1)
			if flip<= 0.3:
				M.remove_row(row)
			elif flip <= 0.6:
				M.prune_row(row, 0.5)

	M.write('out.haps.dat')
  ```
