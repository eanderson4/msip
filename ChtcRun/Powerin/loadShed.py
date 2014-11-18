#!./python273/bin/python

import sys
from tools import *

if __name__=="__main__":
    if len(sys.argv) > 2:
        nominaldemand = 3668
        print "Analyzing "+argv[1]+" and outputing results to "+argv[2] )
        loadshedanalysis( argv[1], argv[2], nominaldemand )

    else:
        print "Usage: loadShed.py < ???.dem > < ???.lsa >  (Statistical analysis of load shed data)"

