#!/progs/gurobi/linux64/bin/python2.7

import sys
import os
import subprocess
import time
import operator
from tools import *

def solveProblem(name):
    print "calling ./msip"
    args = ['./msip', 'input/sim_cplex.in', name]
    p = subprocess.Popen(args)
    p.wait()

def noadd(num, scene, stage, outcome):
    out = "tmp/sim_final.dem"
    outL = "tmp/sim_lines.out"
    total = "tmp/sim.dem"
    totalL = "tmp/lines.out"
    ffout = open(total,'w')
    flout = open(totalL,'w')  
    for i in range(num):
        makeInputFile("./input/sim_cplex.in",scene, stage,outcome)
        solveProblem("")           
        ff = open(out,'r')
        fl = open(outL,'r')
        linesf = ff.readlines()
        linesf.pop(0)
        linesl = fl.readlines()
        linesl.pop(0)
        ffout.writelines( linesf )
        flout.writelines( linesl )
        ff.close()
        fl.close()      
    ffout.close()
    flout.close()

def oneadd(num, trials, scene, stage, outcome):
    ftotal = open('./tmp/dumb_'+str(num)+'.dem','w')
    for i in range(trials):
        makeInputFile("./input/sim_cplex.in",scene, stage,outcome)
        solveProblem("tmp/dumb_"+str(num)+".cap")
        fout = open('tmp/dumb_'+str(num)+'_final.dem','r')
        linesf = fout.readlines()
        linesf.pop(0)
        ftotal.writelines( linesf )
        fout.close()
    ftotal.close()

def countoutput(num, scenarios):
    count('tmp/lines.out','tmp/lineOutage_'+str(num)+'.lao' , scenarios )

def createdumb(num, MW, k):
    dumb('tmp/lineOutage_'+str(num)+'.lao','tmp/dumb_'+str(num)+'.cap', MW, k)

def runanalysis(num):
    loadshedanalysis('tmp/dumb_'+str(num)+'.dem','tmp/dumb_'+str(num)+'.lsa',3668)

def outputdata(fout,ls):
    f = open(fout,'w')
    f.write('run smp ex sd se min max var cvar\n')
    for num in ls:
        ts = readlsa('tmp/dumb_'+str(num)+'.lsa')
        f.write(str(num)+'\t')
        for t in ts:
            f.write(t+'\t')
        f.write('\n')     
    f.close()


if __name__=="__main__":
    print "Usage: trial.py  Run trial on cascading system"
    ls = [ 2 ]
    MW = 25
    k = 10
    trials = 5       
    conting = 4
    stages = 6
    outcomes = [ 10,5,2,1,1,1 ] 
    for n in ls:
        noadd(n, conting, stages, outcomes)

        scenarios = conting*stages*n*10
        countoutput(n,scenarios)
        print "here"
#        createdumb(n,MW,k)
#        oneadd(n,trials, conting, stages, outcomes)
#        runanalysis(n)
#    outputdata('output/trial.dat',ls)

