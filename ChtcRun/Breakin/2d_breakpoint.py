#!/progs/gurobi/linux64/bin/python2.7

import sys
import os
import operator
from tools import *
from power import *

def makeCapFile(lines,cap1):
    f = open( 'LIST_'+lines[0], 'w' )

    for c1 in cap1:
        name = '2d'+lines[0]+'.'+str(c1)
        f.write(name+' ')
        if not os.path.exists( name ):
            os.mkdir( name )
        capfile = '2d'+name +'/additions.cap'
        fout = open(capfile,'w')
        limit0=float(a.getBranchLimit(lines[0]))
            
        fout.write('Design Capacity -individual\n')
        for line in lines:

            weight=float(a.getBranchLimit(line))/limit0
            fout.write(line+' '+str(weight*c1)+'\n')

    f.close()
    fout.close()


def outputData(lines,cap1):
    line = lines[0]
    inputlocation = '../step0/'
    f = open('2d_'+line+'.dat','w')
    f.write('run smp ex sd se min max var cvar\n')
    for c1 in cap1:
        name = '2d'+ line+'.'+str(c1)
        ts = readlsa(inputlocation + name+'/want.lsa')
        f.write(str(c1)+'\t')
        for t in ts:
            f.write(t+'\t')
        f.write('\n')
    f.close()

    fl = open('2d_'+line+'.ldat','w')
    fls = open('2d_'+line+'.lsdat','w')
    fl.write('name ')
    for i in range(185):  # FOR GRID 2
        fl.write( str(i) +' ')
    fl.write('\n')
    for c1 in cap1:
        name = '2d'+line+'.'+str(c1)
        ts = readlao(inputlocation+name+'/want.lao')
        fl.write(str(c1)+'\t')
        fls.write(str(c1)+'\t')
        
        for key in sorted(ts.iterkeys()):
            fl.write(str(ts[key])+'\t')      

        for key in sorted( ts.iteritems(), key=operator.itemgetter(1), reverse=True ):
            fls.write(str(key[0])+'\t')

        fl.write('\n')
        fls.write('\n')

    fl.close()
    fls.close()
    

    fc = open('2d_'+line+'.cdat','w')
    fc.write('name')
    beginning = True

    for c1 in cap1:
        name = '2d'+line+'.'+str(c1)
        ts = readcap(name+'/additions.cap')
        fc.write(str(c1)+'\t')

        if beginning:
            for t in ts:
                fc.write('\t'+t)
            fc.write('\n')
            beginning = False
            
        fc.write(str(c1))
        for t in ts.keys():
            fc.write('\t'+ts[t])
        fc.write('\n')
    fc.close()



def cleanFiles(lines,cap):
    line = lines[0]
    for c in cap:
        name = '2d'+line+'.'+str(c)
        if os.path.exists(name):
            os.remove(name+'/additions.cap')
            os.rmdir(name)
            
if __name__=="__main__":


    if len(sys.argv) > 2:

        a = Grid()
        a.readData( sys.argv[2] )


#        lines = [ '31', '48', '84', '85', '15', '97', '77', '98', '37','106', '181', '113', '141', '137' ]
        if len(sys.argv) > 3:
            fin = open( sys.argv[3] , 'r')
            linest = fin.readline()
            check = linest.split('\t')
        else:
            fin = open( "LINES" , 'r' )
            linest = fin.readline()
            check = linest.split(' ')        

        print linest
        print check
        fin.close()



        subset = check[0 : 104 ]

        fsubset = open('breakpoint.lines','w')
        for line in subset:
            fsubset.write(line + ", " )
        fsubset.close()

       
#        for line in subset:
            
        a.printBranch( subset[0] )
        Limit = a.getBranchLimit( subset[0] )
        print Limit

        cs = [i*float(Limit)/200 for i in range(601)]
        cs_r = [ round(elem, 2) for elem in cs ]
        print cs_r



        if sys.argv[1] == '-p':
            print "Process"
            outputData(subset,cs_r)
        if sys.argv[1] == '-c':
            print "Make condor submit structure for dag"
            makeCapFile(subset,cs_r)
        if sys.argv[1] == '-clean':
            print "Clean condor submit structure"
            cleanFiles(subset,cs_r)
            


    else:
        print "Usage: consub_1d.py <operation> <gridfile>  (Interface with condor)"
        print "            -p process files"
        print "            -c make condor submit"
        print "            -l count line outages"



    


    
    
    

