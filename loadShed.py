#!/usr/bin/python

import sys
from tools import *

if __name__=="__main__":
    if len(sys.argv) > 2:
        nominaldemand = 3668
        n1 = sys.argv[1]
        n2 = sys.argv[2]
#        print "Analyzing "+sys.argv[1]+" and outputing results to "+sys.argv[2] 
        loadshedanalysis( sys.argv[1], sys.argv[2], nominaldemand )
        a=readlsa(sys.argv[2])
        for i in a:
            print(i+' '),
    else:
        print "Usage: loadShed.py < ???.dem > < ???.lsa >  (Statistical analysis of load shed data)"

