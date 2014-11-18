#!./python273/bin/python

import sys
from tools import *


if __name__=="__main__":

    
    if len(sys.argv) or len(sys.stdin) >1:
        caps = sys.stdin

        f = open('output/done.dat','w')
        f.write('name smp ex st se min max var cvar\n')
        print "Processing "
        for cap in caps:
            print cap
            a=readlsa(cap.rstrip())
            print a
            name = cap.split('done')[1].split('.')[0]
            f2 =open('output/done'+name+'.ls','w')
            f2.write('name smp ex st se min max var cvar\n')
            print name

            f.write(name)
            f2.write(name)
            for b in a:
                f.write(' '+b)
                f2.write(' '+b)
            f.write('\n')

            f2.close()

            

    
    else:
        print "Usage: process.py < list of lines >  (creates .dat file to sumarrize a list of .lsa files"
