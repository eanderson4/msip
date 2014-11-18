#!/progs/gurobi/linux64/bin/python2.7

import sys
import os
import shutil
from cap import *
from tools import *
from power import *

if __name__=="__main__":


    if len(sys.argv) > 2:

        print sys.argv

        outputLocal = '../mostOutages'

        if sys.argv[1] =='-step':
            pointin = sys.argv[2]
            linesin = sys.argv[3]
            print 'Searching for better points'
            print 'Open '+pointin+' as initial point'
            print 'and search lines from '+linesin

            ts = readcap(pointin)
            
            if '.lines' in linesin:
                fl = open( linesin, 'r')
                lines = fl.readline().split(", ")
                print lines
                if '' in lines:
                    lines.remove('')
                stepLines( lines, ts, 31 )
                fl.close()
            if '.group' in linesin:
                fl = open( linesin, 'r')
                groups = fl.readline().split("/ ")
                
                print groups
                stepGroups( groups, ts )
                fl.close

        else:
            if len(sys.argv) > 3:
                outputLocal = sys.argv[3]
                iteration = sys.argv[3].split('step')[1]
                print 'Iteration: ',iteration


        if sys.argv[1] == '-point':
            print "Find good point and output to point.cap"
            print "Find lines to search from new point and output to search.lines"
            test = Cap( sys.argv[2], outputLocal)
                        
            test.params(.1,.2)
#            ss=test.getStableSet()
            budget=500
            point=test.getGreedyPoint()
            print point

            name = outputLocal+point[0]+'_'+point[1]+'/add.cap'

            ts = readcap( name )

            f = open('point.cap','w')
            f.write('Design Capacity -individual')



            for t in ts.keys():
                f.write('\n' + t + ' ' + ts[t])
            f.close()

            namelao = outputLocal+point[0]+'_'+point[1]+'/want.lao'

            #Find lines to search
            problines = readlao( namelao )

            flao = open('search.lines','w')
            print  problines
            for line in problines.keys():
                Threshold = 2500
                if problines[line] > Threshold:
                    flao.write(str(line)+', ')
                    print line

            flao.close()


            stub = outputLocal + point[0]+'_'+point[1]
            

            shutil.copy( namelao, 'point.lao')
            shutil.copy( stub+'/want.lsa','point.lsa')



        if sys.argv[1] == '-c':
            

            test = Cap( sys.argv[2], outputLocal)

            test.params(.1,.2)
            ss=test.getStableSet()
            budget=500
            picks=test.getGreedySet(budget)
            print picks

            test.createFiles(picks,budget)

        if sys.argv[1] == '-picks':
            test = Cap( sys.argv[2], outputLocal )
            test.params(.1,.2)
            ss=test.getStableSet()
            budget=500
            picks=test.getGreedySet(budget)
            print picks
            f = open('allocate.picks','w')
            for p in picks:
                f.write(str(p)+' ')
            f.close()

#CONDOR SUBMIT first allocation step
           
        if sys.argv[1] == '-clean':
            fin = open( sys.argv[2] , 'r' )
            line = fin.readline()
            print line

            names = [ n for n in line.split(", ") ]
            names.remove( '' )

            print names
            for name in names:
#                print name
                if os.path.exists( name ):
                    shutil.rmtree( name )

        if sys.argv[1] == '-p':
            fin = open( sys.argv[2] , 'r' )
            line = fin.readline()
            print line

            names = [ n for n in line.split(", ") ]
            names.remove( '' )

            stubs = set([ n.split('_')[0] for n in names ])
            print stubs
            

            for s in stubs:
                f = open( s+'.dat','w' )
                f.write('run smp ex sd se min max var cvar\n')

                for name in names:
                    if s+'_' in name:
#                        print outputLocal+'/'+name
                        ts = readlsa(outputLocal+'/'+name+'/want.lsa')
                        f.write(name.split('_')[1]+'\t')
                        for t in ts:
                            f.write(t+'\t')
                        f.write('\n')
                f.close()

                fl = open(s+'.ldat','w')
                fls = open(s+'.lsdat','w')
                fl.write('name ')
                for i in range(185):  # FOR GRID 2
                    fl.write( str(i) +' ')
                fl.write('\n')
                for name in names:
                    if s+'_' in name:
                        ts = readlao(outputLocal+'/'+name+'/want.lao')
                        fl.write(name.split('_')[1]+'\t')
                        fls.write(name.split('_')[1]+'\t')
        
                        for key in sorted(ts.iterkeys()):
                            fl.write(str(ts[key])+'\t')      
                        
                        for key in sorted( ts.iteritems(), key=operator.itemgetter(1), reverse=True ):
                            fls.write(str(key[0])+'\t')

                        fl.write('\n')
                        fls.write('\n')

                fl.close()
                fls.close()
                
                fc = open(s+'.cdat','w')
                fc.write('name')
                
                beginning = True

                for name in names:
                    if s+'_' in name:
                        ts = readcap(outputLocal+'/'+name+'/add.cap')
                        
                        if beginning:
                            for t in ts:
                                fc.write('\t'+t)
                            fc.write('\n')
                            beginning = False
                            
                        fc.write( name.split('_')[1])
                        for t in ts.keys():
                            fc.write('\t'+ts[t])
                        fc.write('\n')
                fc.close()
                

            print stubs


    else:
        print "Usage:  allocate <operation> <line in files>"
        print "                 -c (create files)"
        print "                 -p (process files)"
        print "                 -clean (clean files)"
        print "                 -step <point.cap> <search.lines>"
        
