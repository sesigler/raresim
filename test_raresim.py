import unittest
from rareSim import sparse
from header import *

class testRaresim(unittest.TestCase):
    
    def test_readExpected(self):
        bins = read_expected('./testData/testBins.txt')
        self.assertEqual(bins[3], (3,5,2))

    def test_getBin(self):
        bins = read_expected('./testData/testBins.txt')
        self.assertEqual(get_bin(bins, 4), 2)

    def test_rowNum(self):
        M = sparse(None)
        M.load('./testData/test.haps.sm')
        self.assertEqual(M.row_num(0), 3)
        self.assertEqual(M.row_num(1), 0)
        self.assertEqual(M.row_num(8), 1)

    def test_read_legend(self):
        legend_header, legend = read_legend('./testData/test.legend')
        self.assertEqual(legend_header, ['id','position','a0','a1'])
        self.assertEqual(legend[0], {'id':'19:14492336_A_G', 'position':'1', 'a0':'A', 'a1':'G'})

    def test_assign_bins(self):
        legend_header, legend = read_legend('./testData/test.legend')
        M = sparse(None)
        M.load('./testData/test.haps.sm')
        bins = read_expected('./testData/testBins.txt')

        bin_h = assign_bins(M, bins, legend, False)
        correct_assignment = {
            0 : [2, 3, 4, 7, 8, 10, 11, 12, 13, 14, 15, 18, 21, 22, 23, 24, 25, 26, 29],
            1 : [9, 16, 17, 19, 20, 27, 28, 30],
            2 : [0, 5]
        }
        self.assertEqual(bin_h, correct_assignment)



if __name__ == '__main__':
    unittest.main()