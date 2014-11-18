#!/progs/gurobi/linux64/bin/python2.7

import sys
import os
import operator
from tools import *
from power import *

def makeCapFile(line,cap1):
    f = open( 'LIST_'+line, 'w' )

    for c1 in cap1:
        name = line+'.'+str(c1)
        f.write(name+' ')
        if not os.path.exists( name ):
            os.mkdir( name )
        capfile = name +'/additions.cap'
        fout = open(capfile,'w')
            
        fout.write('Design Capacity -individual\n')
        fout.write(line+' '+str(c1))

    f.close()
    fout.close()


def outputData(line,cap1):
    inputlocation = '../step0/'
    f = open('1d_'+line+'.dat','w')
    f.write('run smp ex sd se min max var cvar\n')
    for c1 in cap1:
        name = line+'.'+str(c1)
        ts = readlsa(inputlocation + name+'/want.lsa')
        f.write(str(c1)+'\t')
        for t in ts:
            f.write(t+'\t')
        f.write('\n')
    f.close()

    fl = open('1d_'+line+'.ldat','w')
    fls = open('1d_'+line+'.lsdat','w')
    fl.write('name ')
    for i in range(185):  # FOR GRID 2
        fl.write( str(i) +' ')
    fl.write('\n')
    for c1 in cap1:
        name = line+'.'+str(c1)
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
    

    fc = open('1d_'+line+'.cdat','w')
    fc.write('name')
    beginning = True

    for c1 in cap1:
        name = line+'.'+str(c1)
        ts = readcap(inputlocation+name+'/add.cap')
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



def cleanFiles(line,cap):
    for c in cap:
        name = line+'.'+str(c)
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

       
        for line in subset:
            
            a.printBranch( line )
            Limit = a.getBranchLimit( line )
            print Limit

            cs = [i*float(Limit)/10 for i in range(31)]
            cs_r = [ round(elem, 2) for elem in cs ]
            print cs_r



            if sys.argv[1] == '-p':
                print "Process"
                outputData(line,cs_r)
            if sys.argv[1] == '-c':
                print "Make condor submit structure for dag"
                makeCapFile(line,cs_r)
            if sys.argv[1] == '-clean':
                print "Clean condor submit structure"
                cleanFiles(line,cs_r)
            


    else:
        print "Usage: consub_1d.py <operation> <gridfile>  (Interface with condor)"
        print "            -p process files"
        print "            -c make condor submit"
        print "            -l count line outages"



    


    
    
    

