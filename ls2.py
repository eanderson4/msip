#!/usr/bin/python

import sys
from tools import *

if __name__=="__main__":
    if len(sys.argv) > 2:
        nominaldemand = 3668
        n1 = sys.argv[1]
        n2 = sys.argv[2]
        f = open("add.cap")
        lines =f.readlines()
        cap1 = lines[1].split(' ')[1]
        cap2 = lines[2].split(' ')[1]
#        print lines
#        print "Analyzing "+sys.argv[1]+" and outputing results to "+sys.argv[2] 
        loadshedanalysis( sys.argv[1], sys.argv[2], nominaldemand )
        a=readlsa(sys.argv[2])
        print(cap1.rstrip()+' '),
        print(cap2.rstrip()+' '),
        for i in a:
            print(i+' '),
    else:
        print "Usage: loadShed.py < ???.dem > < ???.lsa >  (Statistical analysis of load shed data)"

