#Power System Tools

import random
import os
from power import *

def makeInputFile(fo,scenarios, stages, outcomes):  #Make input file for msip
    fout = open(fo,"w")
    fout.write("name=sim\n\n")
    fout.write("lineoutage=yes\n")
    
    fout.write("Problem and Solve Method\n")
    fout.write("solvemethod=cplex\n")
    fout.write("problemtype=sim\n")

    fout.write("Power Grid\n")
    fout.write("gridfile=input/grid2.gr\n\n")
    
    fout.write("Line Failure Distribution\n")
    fout.write("p=.5\n")
    fout.write("L=.98\n\n")
    
    fout.write("Scenario Tree\n")
    fout.write("stages="+str(stages)+"\n")
    fout.write("outcomesV=")
    print outcomes
    for out in outcomes:
        fout.write(str(out)+" ")
    fout.write("\n")
    fout.write("scenarios="+str(scenarios)+"\n\n")

    fout.write("Outages\n")
    fout.write("a=12 14 34 11\n")
    fout.write("b=12 2 21\n")
    fout.write("c=82 5 25 34\n")
    fout.write("d=13 24\n\n")

    i = random.randint(12045,1205015)
    fout.write("Random Numbers\n")
    fout.write("seed="+str(i)+"\n\n")

    fout.close()
    print "Wrote to datafile "+ fo


def count(fi, fo, scale):  # "Usage: count.py <line outage file> <output file> <scale>"
    x={'0':0}
    for i in range(186):
        x[str(i)]=0
    fn = open(fo,'w')
    fn.write('line outages\n')
    bigdog=0.1
    f = open(fi,'r')
    
    lines = f.readline()

    [n,p,v] = lines.partition(' ')
    x[n]=1+x.get(n,0)

    while (v):
        [n,p,v] = v.partition(' ')
        x[n]=1+x.get(n,0)
        if (bigdog<x[n]):
            bigdog=x[n]
            
    print x
    sorted_x = sorted(x.items(), key=lambda x: x[1])
    sorted_x.reverse()
    print sorted_x

    bigdog=float(scale)

    for i in sorted_x:
        fn.write(i[0]+" "+str(i[1])+"\n")

    print "BIG: "+str(bigdog)
    fn.close()
    f.close()
        
def dumb(fi, fout, MW, k):   # Read in line outage file and output capacity file
    
    fin = open(fi,'r')
    l = fin.readline()
    print l
    lines = fin.readlines()

    x={'0':0}
    for i in range(186):
        x[str(i)]=0

    rank=['0']

    for l in lines:
        [ln,p,e] = l.partition(" ")
        [hz,p,e] = e.partition("\n")
        x[ln]=hz
        rank.append(ln)
        print ln,hz

    rank.pop(0)

    MWb = MW * 29600/100
    print "Per Line: ",MWb/k,", Lines: ",k
    
    rank = rank[0:k]
    print rank
    
    f = open(fout,'w')
    f.write("Design Capacity -individual\n")
    for line in rank:
        f.write(str(line))
        f.write(' ')
        f.write(str(MWb/k))
        f.write('\n')
    f.close()

def loadshedanalysis( fi, fo, nom ):  #Read in <.dem> file and preform analysis and output to <.lsa>
    f = open(fi,'r')
    for line in f:
        len(line)
    ls = line.split("  ")
    size = len(ls) -1
    ls = ls[0:size]
    ls.sort()
#    print "\nDemand Served"
#    print ls
    total=0
    ls1 = []
    for item in ls:
        ls1.append(nom-float(item))
        total = total + nom - float(item)
    sq=0
    avg=total/size
    i = int(.05*size)
    count=0
    c2=0
    t2=0
    for item in ls1:
        count=count+1
        if(count<=i):
            c2=c2+1
            t2=t2+item
        sq = (item-avg)*(item-avg)/size + sq
    se=sq**.5/size**.5
    avg=total/size

    fout = open( fo, 'w')
    fout.write( "Samples: " +str(size) )
    fout.write( "\nAverage: " + str(avg)+ "  CI 95% [ " + str(avg-se*1.96) + ", " + str(avg+se*1.96) +" ]" )
    fout.write( "\nStandard Deviation: " + str(sq**.5) )
    fout.write( "\nStandard Error: " + str(se) )
    fout.write( "\nMin: " + str(min(ls1)) )
    fout.write( "\nMax: " + str(max(ls1)) )
    fout.write( "\n95% V@R: " + str(ls1[i]) )
    fout.write( "\n95% CV@R: " + str((t2/c2)) )
    
    f.close()
    fout.close()

def readlsa(fi):   #  Read load shed analysis file and return data
    data = [ 0, 0, 0, 0, 0, 0, 0, 0 ]
    #      [ Samples, Expect, St Dev, St Err, Min, Max, Var, CVar ]
    f = open(fi,'r')

    samples = f.readline()
    [a,samples] = samples.split(' ')
    [samples, a] = samples.split('\n')
    data[0] = samples

    mean = f.readline()
    mean =  mean.split(' ')[1]
    data[1] = mean

    stdv = f.readline()
    stdv = stdv.split(' ')[2].split('\n')[0]
    data[2] = stdv

    sterr = f.readline()
    sterr = sterr.split(' ')[2].split('\n')[0]
    data[3] = sterr

    mn = f.readline()
    mn = mn.split(' ')[1].split('\n')[0]
    data[4] = mn

    mx = f.readline()
    mx = mx.split(' ')[1].split('\n')[0]
    data[5] = mx

    var = f.readline()
    var = var.split(' ')[2].split('\n')[0]
    data[6] = var

    cvar = f.readline()
    cvar = cvar.split(' ')[2].split('\n')[0]
    data[7] = cvar
    f.close()

    return data

def readlao(fi):   #  Read load shed analysis file and return data
    data = { '-1':0 }
    #      [ line:outages ]
    f = open(fi,'r')

    lines = f.readlines()

    
    for line in lines:
        if 'line' in line:
            testr=1
        else:
            [a, b] = line.split(' ')
            data[int(a)] = int(b.split('\n')[0])

    data.pop('-1')
    return data
    
def readcap(fi):   #  Read load shed analysis file and return data
    data = { '-1':0 }
    #      [ line:capacity addition ]
    f = open(fi,'r')
    f.readline()
    lines = f.readlines()

    
    for line in lines:
        data[ line.split(" ")[0] ] = line.split(" ")[1].split('\n')[0]


    data.pop('-1')
    
    return data
    

def stepLines( lines, ts, numPoints):
    budget = 500
    used = 0
    norm = (numPoints-1)/3
    for t in ts.keys():
        print t,ts[t]
        used = float(used) + float(ts[t])
        
    a = Grid()
    a.readData( 'grid2.gr' )

    flist = open('allocate.lines','w')
    linenumber=1
    for line in lines:
        a.printBranch( line )
        Limit = a.getBranchLimit( line )
        print Limit
        if float(Limit) > float(budget)/3:
            Limit = float(budget)/3
        print Limit
        cs = [i*float(Limit)/norm for i in range(numPoints)]
        cs_r = [ round(elem, 2) for elem in cs ]
        print cs_r
        
        basename='n'+str(linenumber)
        linenumber=linenumber+1
        for i in range(numPoints):
            name = basename+'_'+str(i)
            flist.write(name+', ')
            if not os.path.exists( name ):
                os.mkdir( name )
            capfile = name+'/additions.cap'
            fout = open(capfile,'w')
            fout.write('Design Capacity -individual\n')
            
            newbudget = budget - cs_r[i]
            w = newbudget/budget
            print w,newbudget, used, budget
            for t in ts.keys():
                final = float(ts[t])*w
                fout.write(t+' '+str(final)+'\n')
                
            fout.write(line+' '+str(cs_r[i])+'\n')
                    
            fout.close()

def stepGroups(groups, ts):
    budget = 500
    used = 0
    for t in ts.keys():
        print t,ts[t]
        used = float(used) + float(ts[t])
        
    a = Grid()
    a.readData( 'grid2.gr' )

    flist = open('step.lines','w')
    linenumber=1
    for group in groups:
        print group

        gs = group.split(", ")
        groupcap = 0
        for line in gs:
            a.printBranch( line )
            Limit = a.getBranchLimit( line )
            print Limit
            groupcap = groupcap + float(Limit)

#        groupweights = [ float(a.getBranchLimit( line ))/groupcap for line in gs ]
        groupweights = { '-1':0 }
        for line in gs:
            groupweights[ line ] = float(a.getBranchLimit( line ))/groupcap
        groupweights.pop('-1')
        print groupweights
        
        w1 = [float(i)/30 for i in range(31)]
        w2 = [1-float(i)/30 for i in range(31)]
        
        ef1 = [w*budget for w in w1]
        ef2 = [w*budget for w in w2]

        print ef1
        print ef2

        basename='n'+str(linenumber)
        linenumber=linenumber+1
        for i in range(31):
            name = basename+'_'+str(i)
            flist.write(name+', ')
            if not os.path.exists( name ):
                os.mkdir( name )
            capfile = name+'/additions.cap'
            fout = open(capfile,'w')
            fout.write('Design Capacity -individual\n')

            newbudget = ef2[i]
            w = newbudget/used
            for t in ts.keys():
                final = float(ts[t])*w
                fout.write(t+' '+str(final)+'\n')

            for line in gs:
                final = ef1[i]*groupweights[line]
                fout.write(line+' '+str(final)+'\n')
            fout.close()
                         
"""
        a.printBranch( line )
        Limit = a.getBranchLimit( line )
        print Limit
        cs = [i*float(Limit)/10 for i in range(31)]
        cs_r = [ round(elem, 2) for elem in cs ]
        print cs_r
        
        for i in range(31):
            
            

            w = newbudget/budget
            print w,newbudget, used, budget
            for t in ts.keys():
                final = float(ts[t])*w
                fout.write(t+' '+str(final)+'\n')
                
            fout.write(line+' '+str(cs_r[i])+'\n')
                    
            fout.close()
"""    
