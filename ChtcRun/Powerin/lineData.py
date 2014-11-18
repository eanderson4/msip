#!/progs/gurobi/linux64/bin/python2.7
#####!./python273/bin/python

import sys
import operator
from tools import *


if __name__=="__main__":

    if len(sys.argv) >2:
        "Processing "+sys.argv[1]+ " into " +sys.argv[2]
        f = open( sys.argv[2], 'w' )
        ts = readlao( sys.argv[1] )
        f.write('name\t')
        print ts

        for key in sorted( ts.iteritems(), key=operator.itemgetter(1), reverse=True ):
            f.write(str(key[0])+'\t')
        f.write('\n')

        f.write('name\t')
        for key in sorted(ts.iterkeys()):
            f.write(str(ts[key])+'\t')
        f.write('\n')

        f.close()
        


    else:
        print "Usage: lineData.py  list of <.lao> < ???.ldat>  (Outputs list of <lao> to <ldat> file))"
