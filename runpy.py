#!/usr/bin/python

from averageFlows import *

#f[s][j][e], p[s][j][e]
#  s - stage
#  j - line number
#  e - scenario

mc = getMaxCap('grid2.gr')
f = getFlows('done.lf')
p = [ probabilityTransform( s, f, mc ) for s in range(len(f))]

ns = len(f[0][0])  #Number of scenarios
nominaldemand=3668
count('done.lin','done.lao',ns)
loadshedanalysis( 'done.dem','done.lsa',nominaldemand )


#Print Probabilities to file
stage=1
#writePob(p,stage)
writeAndDisplayPob(f,p,stage)
writeLfi(f,stage)

#Get Average Probabilities
avgp = {-1:-1} 
for s in range(len(f)):
    avgp[s] = [ sum(pj)/len(pj) for pj in p[s] ]
del avgp[-1]

#Write Average Probabilities
fout = open('done.pba','w')
fout.write('line prob\n')
#for s in range(len(avgp)):
for j in range(len(avgp[stage])):
    fout.write(str(j) +' '+ str(avgp[stage][j])+'\n')    
fout.close()

#Get Average Probabilities
avgf = {-1:-1} 
for s in range(len(f)):
    avgf[s] = [ sum(fj)/len(fj) for fj in f[s] ]
del avgf[-1]

#Write Average Probabilities
fout = open('done.lfa','w')
fout.write('line flow\n')
#for s in range(len(avgp)):
for j in range(len(avgf[stage])):
    fout.write(str(j) +' '+ str(avgf[stage][j])+'\n')    
fout.close()






#displayLineInfo(f,p,mc,stage,15)
#print avgp[stage][15]
#writeAndDisplayPob(f,p,stage)
#for j in range(30):
#    displayLineInfo(f,p, mc,1,j)

#lao = countOnly('done.lin',ns)
#print lao


