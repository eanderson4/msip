#!/usr/bin/python

from averageFlows import *

#f[s][j][e], p[s][j][e]
#  s - stage
#  j - line number
#  e - scenario

mc = getMaxCap('grid2.gr')
f = getFlows('done.lf')

max67 = 0
max79 = 0
max115 = 0
for s in range(len(f)):
    for n in range(len(f[1][1])):
        if (max67<f[s][67][n]):
            max67=f[s][67][n]
        if (max79<f[s][79][n]):
            max79=f[s][79][n]
        if (max115<f[s][115][n]):
            max115=f[s][115][n]
#    print f[s][67]
#    print f[s][79]

print len(f)
print len(f[1])
print len(f[1][1])

print max67
print max79
print max115
