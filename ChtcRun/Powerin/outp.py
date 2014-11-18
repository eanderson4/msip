#!/progs/gurobi/linux64/bin/python2.7

import sys
import os
import shutil
from cap import *
from tools import *
from power import *

if __name__=="__main__":


    if len(sys.argv) > 0:

        print sys.argv

        outputLocal = '../opt1/step'

        namescap = [ outputLocal + str(i) + '/point.cap' for i in range(11) ]
        nameslsa = [ outputLocal + str(i) + '/point.lsa' for i in range(11) ]
        nameslao = [ outputLocal + str(i) + '/point.lao' for i in range(11) ]

        print namescap,nameslsa,nameslao

        if True:
            f = open( 'opt1.dat','w' )
            f.write('run smp ex sd se min max var cvar\n')

            for name in nameslsa:
                ts = readlsa(name)
                f.write(name.split('step')[1].split('/point')[0]+'\t')
                for t in ts:
                    f.write(t+'\t')
                f.write('\n')
            f.close()

            fl = open('opt1.ldat','w')
            fls = open('opt1.lsdat','w')
            fl.write('name ')
            for i in range(185):  # FOR GRID 2
                fl.write( str(i) +' ')
            fl.write('\n')
            for name in nameslao:
                ts = readlao(name)
                fl.write(name.split('step')[1].split('/point')[0]+'\t')
                fls.write(name.split('step')[1].split('/point')[0]+'\t')
        
                for key in sorted(ts.iterkeys()):
                    fl.write(str(ts[key])+'\t')      
                        
                for key in sorted( ts.iteritems(), key=operator.itemgetter(1), reverse=True ):
                    fls.write(str(key[0])+'\t')

                fl.write('\n')
                fls.write('\n')

            fl.close()
            fls.close()
                
            fc = open('opt1.cdat','w')
            fc.write('name')
                
            beginning = True

            for name in namescap:
                ts = readcap(name)
                        
                if beginning:
                    for t in ts:
                        fc.write('\t'+t)
                    fc.write('\n')
                    beginning = False
                            
                fc.write(name.split('step')[1].split('/point')[0]+'\t')
                for t in ts.keys():
                    fc.write('\t'+ts[t])
                fc.write('\n')
            fc.close()
                




    else:
        print "Usage:  allocate <operation> <line in files>"
        print "                 -c (create files)"
        print "                 -p (process files)"
        print "                 -clean (clean files)"
        print "                 -step <point.cap> <search.lines>"
        
