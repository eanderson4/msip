#!./python273/bin/python

import sys
from tools import *


if __name__=="__main__":

    if len(sys.argv) >2:
        "Processing "+argv[1]+ " into " +argv[2]
        trials = 10000
        count( argv[1], argv[2], trials )
        
    else:
        print "Usage: countLines.py < ???.lin > < ???.lao>  (Counts line outages)"
