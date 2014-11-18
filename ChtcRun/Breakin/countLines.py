#!./python273/bin/python

import sys
from tools import *


if __name__=="__main__":

    if len(sys.argv) >2:
        n1 = sys.argv[1]
        n2 = sys.argv[2]
        "Processing "+n1+ " into " +n2
        trials = 10000
        count( n1, n2, trials )
        
    else:
        print "Usage: countLines.py < ???.lin > < ???.lao>  (Counts line outages)"
