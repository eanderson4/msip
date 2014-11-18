#!/usr/bin/python

import sys
from tools import *


def getMaxCap(fi):
    g = Grid()
    g.readData('grid2.gr')
    lines=range(int(g.getNumBranch()))

    #original max cap
    maxcap = [float(g.getBranchLimit(str(line))) for line in lines]

    #Capacity from capacity addition file
    capplus = readcap('add.cap')
    for cap in capplus:
        n=int(cap)
        maxcap[n]=maxcap[n] + float(capplus[cap])
        
    return maxcap


def probabilityTransform( s, flows, maxcap ):
    L=float(.95)
    U=float(1)
    p=float(.5)
    lines=range(len(flows[0]))

    zerone=[] #Transform flow to zero one based off capacity
    prob=[] #Probability after transform

    for j in lines:
        mc=float(maxcap[j])
        zerone.append([float(flow)/mc for flow in flows[s][j]])
        prob.append( [ (float(zo)-L)*p/(U-L)*float(zo>L) for zo in zerone[j] ])
            
    return prob           

    
def getFlows( n1 ):  #Get Flows from data file

    "Processing "+n1
    fi = n1
    
    f = open(fi,'r')
    
    lines = f.readlines()
    
    events = { -1:'-1' }
    count = 0

    for line in lines:
        count = count+1  #Number of stages
        events[count] =  line.split(' - ')[:-1]  #line flow by stage
    del events[-1]

    
    first = True
    averageFlow={'-1':'-1'}
    flows={'-1':'-1'}
        
    for e in range(1,count+1):
        stages = len(events[e])
        if first:
            for s in range(stages):  #start list of flows for each line by stage
                flows[s] =  [  [abs(float(fj))] for fj in events[e][s].split(' ') ]
            first=False
        else:
            for s in range(stages):
                lineflows=events[e][s].split(' ')
                numlines=len(lineflows)
                for j in range(numlines): 
                    flows[s][j].append(abs(float(lineflows[j])))
        

    del flows['-1']            

    return flows


#OUTPUT FUNCTIONS

def writePob( p, s):
    fout = open('done.pob','w')
    fout.write('line prob\n')
    for j in range(len(p[s])):
        for prob in p[s][j]:
            if prob == -0.0:
                pr = '0'
            else:
                pr = str(prob)
            fout.write(str(j) +' '+ pr+'\n')    
    fout.close()

def writeLfi( f, s):
    fout = open('done.lfi','w')
    fout.write('line flow\n')
    for j in range(len(f[s])):
        for flow in f[s][j]:
            fout.write(str(j) +' '+ str(flow) +'\n')    
    fout.close()

def writeAndDisplayPob( f,p, s ):
    fout = open('done.pob','w')
    fout.write('line prob\n')
#    print s
    for j in range(len(f[s])):
#        print s,j
#        print "========="
#        print f[s][j]
#        print p[s][j]
#        print "=========\n"
#        print len(p[s][j])
        for prob in p[s][j]:
            if prob == -0.0:
                pr = '0'
            else:
                pr = str(prob)
            fout.write(str(j) +' '+ pr+'\n')
    
    fout.close()

def displayLineInfo( f, p, mc, stage, line):
    numscene=len(f[stage][line])
    print 'Stage',stage,'for line',line, 'with',numscene,'scenarios'
    print mc[15],'\tmax cap'

    f_filter = [ fn for fn in f[stage][line] if fn >0 ]
    p_filter = [ pn for pn in p[stage][line] if pn >0 ]
    print len(f[stage][line]),'\tflows'
    print len(f_filter),'\tflows greater than 0'
    print len(p_filter),'\tprob greater than 0'

    print '\navg flow \t[ flow1, flow2, ..]'
    avgf = sum(f[stage][line])/numscene 
    avgp = sum(p[stage][line])/numscene 
    print avgf,f[stage][line]
    print '\navg prob \t[ prob1, prob2, ..]'
    print avgp,p[stage][line]

