#!/usr/bin/python

import sys

if len(sys.argv)<2:
    exit(0)

print sys.argv[1]

NomDem=3668

oldname=sys.argv[1].split('.')[0]
ffst=open(oldname+'.fst','w')
ffst.write('numlines ex\n')

f = open(sys.argv[1])

lines = f.readlines()

holder = []
ind = []
lst = []
lstlim = 1000    #unique combination standard deviation limit
lstmin = []
lstminlim = 2300  #unique combination minimum limit
lstex = []
#lstexmax = 6400 - 2.9*930 # Expected - 2*sd
lstexmax = 3050 - 2.9*230 # Expected - 2*sd
badcasefrac = .575 #Want line to be in at least this fraction of bad scenarios

info = { '-1':['-1'] }
ct = { '-1':['-1'] }
ex = { '-1':['-1'] }
var = { '-1':['-1'] }
mn = { '-1':['-1'] }
med = { '-1':['-1'] }
mx = { '-1':['-1'] }

numcount = {'-1':['-1']}
numscore = {'-1':['-1']}
nums2 = {'-1':['-1']}
numvar = {'-1':['-1']}
nummn = {'-1':['-1']}
nummx = {'-1':['-1']}

paircount = {'-1':['-1']}
pairscore = {'-1':['-1']}
pairs2 = {'-1':['-1']}
pairvar = {'-1':['-1']}
pairmn = {'-1':['-1']}
pairmx = {'-1':['-1']}

count=0
expect=0
demsd=0
lineout=0
linesd=0
for l in lines:
    count=count+1
    sp = l.split(":")
    [s, d] = sp[1].split("-")
    if s not in info:
        info[s] = [ float(d) ]
    else:
        info[s].append(float(d))
    expect=expect+float(d)
    demsd=demsd+float(d)**2
    if s not in holder:
        holder.append(s)
    ls = s.split(" ")
    ls = ls[1:-1]
    lineout=lineout+float(len(ls))
    loadshed = float(NomDem) - float(d) 
    ffst.write(str(len(ls))+' '+str(loadshed)+'\n')
    linesd=linesd+len(ls)**2
#    print ls
    for c in ls:
        if c not in ind:
            ind.append(c)
        if c not in numcount.keys():
            numcount[c] = 1
            numscore[c] = float(d)
            nummn[c] = float(d)
            nummx[c] = float(d)
            nums2[c] = float(d)**2
        else:
            numcount[c] = numcount[c]+1
            numscore[c] = numscore[c] + float(d)
            nums2[c] = nums2[c] + float(d)**2
            if float(d)<nummn[c]:
                nummn[c] = float(d)
            if float(d)>nummx[c]:
                nummx[c] = float(d)
    print s,float(d)

expect=expect/count
demsd=round(demsd/count-expect**2,4)**.5
avglineout=lineout/count
linesd=round(linesd/count-avglineout**2,4)**.5
print holder

print info

del info['-1']
del numcount['-1']
del numscore['-1']
del nums2['-1']
del numvar['-1']
del nummn['-1']
del nummx['-1']



for sam in info.keys():
    numbers = info[sam]

    n = 0
    total = 0
    mn[sam] = 9999
    mx[sam] = 0
    s2 = 0
    
    for num in numbers:
        n=n+1
        total = total + num
        if num < mn[sam]:
            mn[sam] = num
        if num > mx[sam]:
            mx[sam] = num
        s2 = s2 + num**2

    ct[sam] = n
    ex[sam] = total/n
    var[sam] = round(s2/n - ex[sam]**2,4)**.5
    med[sam] = numbers[int(n/2)]


    if (var[sam] > lstlim):
        lst.append(sam)
    
    if mn[sam] < lstminlim:
        lstmin.append(sam)

    if ex[sam] < lstexmax:
        lstex.append(sam)

    print sam,':', ct[sam], ex[sam], var[sam], mn[sam], med[sam], mx[sam]
        
        
    
        
print 'Lines: ',ind
print 'Total: ',count
total = count
print 'Unique combos: ',len(holder)
print 'Unique lines: ',len(ind)

print 'line\tcount\tpercent\tavg\t\ts.d.\tmn\tmx'
for c in numcount.keys():
    nc = numcount[c]
    print c,'\t', numcount[c],'\t',
    numscore[c] = round(float(numscore[c])/numcount[c],2)
    numcount[c] = round(float(numcount[c])*100/count,2)
    numvar[c] = (nums2[c]/nc - numscore[c]**2)**.5
    print numcount[c], '\t', numscore[c],'\t',numvar[c],'\t',nummn[c],'\t',nummx[c]


import numpy

ln = len(ind)
cvLoV = 0
cv = numpy.zeros(shape=(ln,ln))
#cvS = numpy.zeros(shape=(ln,ln))
cvV = numpy.zeros(shape=(1,ln))
#dex={'-1':'-1'}
#count = 0
#for i in ind:
#    dex[i] = count
#    count=count+1
#    print numcount[i]/100
#del dex['-1']


#pairs = []
#for i in range(ln):
#    for j in range(ln):
#        if j>i:
#            pairs.append(ind[i]+' '+ind[j])
pairs = ['1 2']


for l in lines:
    sp = l.split(":")
    [s, d] = sp[1].split("-")
    ls = s.split(" ")
    ls = ls[1:-1]
    lineout = len(ls)
    value=float(d)
    cvLoV = cvLoV + (lineout-avglineout)*(value-expect)
    for ps in pairs:
        [ps1,ps2] = ps.split(' ')
        if ps1 in s and ps2 in s:
            if ps not in paircount.keys():
                paircount[ps] = 1
                pairscore[ps] = float(d)
                pairmn[ps] = float(d)
                pairmx[ps] = float(d)
                pairs2[ps] = float(d)**2
            else:
                paircount[ps] = paircount[ps]+1
                pairscore[ps] = pairscore[ps] + float(d)
                pairs2[ps] = pairs2[ps] + float(d)**2
                if float(d)<pairmn[ps]:
                    pairmn[ps] = float(d)
                if float(d)>pairmx[ps]:
                    pairmx[ps] = float(d)            

    for i in range(ln):
        a = ind[i]
        if a in s:
            aY = 1
        else:
            aY = 0
        cvV[0,i] = (aY - numcount[a]/100)*(value - expect)
        for j in range(ln):
            b = ind[j]
            if b in s:
                bY = 1
            else:
                bY = 0
            cv[i,j] = (aY - numcount[a]/100)*(bY - numcount[b]/100) + cv[i,j]
#            cvS[i,j] = (value - numscore[a])*(value - numscore[b]) + cvS[i,j]

del paircount['-1']
del pairscore['-1']
del pairs2['-1']
del pairvar['-1']
del pairmn['-1']
del pairmx['-1']

cvLoV = round(cvLoV/total,4)
for i in range(ln):
    cvV[0,i] = round(cvV[0,i]/total,4)
    for j in range(ln):
        cv[i,j] = round(cv[i,j]/total,3)
#        cvS[i,j] = round(cvS[i,j]/total,2)
print 'Expect: ',expect
print 'St. Dv.: ',demsd
print 'Line Out Ex: ',avglineout
print 'Line Out Sd: ',linesd
print 'Line Outage Covariance'
print cv
print 'Line Outage Covariance with Demand Served'
print cvV
print 'Number Lines Out Covariance with Demand Served'
print cvLoV
#print cvS

print 'List of unique combinations with standard deviation above',lstlim
for sam in lst:
    print sam,':', ct[sam], ex[sam], var[sam], mn[sam], med[sam], mx[sam]
print 'List of unique combinations with minimum below',lstminlim
for sam in lstmin:
    print sam,':', ct[sam], ex[sam], var[sam], mn[sam], med[sam], mx[sam]
print 'List of unique combinations with expectation below',lstexmax
for sam in lstex:
    print sam,':', ct[sam], ex[sam], var[sam], mn[sam], med[sam], mx[sam]
print 'Pairs'
print pairs
print len(pairs)
print 'pair\tcount\tavg\ts.d.\tmn\tmx'
for ps in paircount.keys():
    pairscore[ps]=pairscore[ps]/paircount[ps]
#    print round((pairs2[ps]/paircount[ps] - pairscore[ps]**2),4)
    pairvar[ps] = round((pairs2[ps]/paircount[ps] - pairscore[ps]**2),4)**.5
    print ps,'\t',paircount[ps],'\t',pairscore[ps],'\t',pairvar[ps],'\t',pairmn[ps],'\t',pairmx[ps]



#print 'List of unique combinations with expectation below',lstexmax
exind = []
excount = {'-1':'-1'}
exfrac = {'-1':'-1'}
c=0
for sam in lstex:
#    print sam,':', ct[sam], ex[sam], var[sam], mn[sam], med[sam], mx[sam]
    c=c+1
    for i in sam.split(' ')[1:]:
        if i not in exind:
            exind.append(i)
        if i not in excount.keys():
            excount[i]=1
        else:
            excount[i]=float(excount[i])+1
        
del excount['-1']
if '' in excount.keys():
    del excount['']
for i in excount.keys():
    exfrac[i] = round(excount[i]/c,4)
del exfrac['-1']

print exind


want = []
for sam in excount.keys():
    print sam,exfrac[sam]
    if exfrac[sam]>badcasefrac:
        want.append(sam)

print want

from itertools import combinations

checklst=[]
for i in range(len(want)):
    for j in combinations(want,i+1):
        checklst.append(j)

print 'Total Combinations: ',len(checklst)

checkCount = {'-1':'-1'}
checkEx = {'-1':'-1'}
checkSd = {'-1':'-1'}
checkMn = {'-1':'-1'}
checkMx = {'-1':'-1'}


for l in lines:
    sp = l.split(":")
    [s, d] = sp[1].split("-")
    ls = s.split(" ")
    ls = ls[1:-1]
    for lst in checklst:
        allin=True
        for i in lst:
            if i not in ls:
                allin=False
        if allin:
            if lst not in checkCount.keys():
                checkCount[lst] = 1
                checkEx[lst] = float(d)
                checkSd[lst] = float(d)**2
                checkMn[lst] = float(d)
                checkMx[lst] = float(d)
            else:
                checkCount[lst] = checkCount[lst] + 1
                checkEx[lst] = checkEx[lst] + float(d)
                checkSd[lst] = checkSd[lst] + float(d)**2
                if float(d) < checkMn[lst]:
                    checkMn[lst] = float(d)
                if float(d) > checkMx[lst]:
                    checkMx[lst] = float(d)

del checkCount['-1']
del checkEx['-1']
del checkSd['-1']
del checkMn['-1']
del checkMx['-1']



for lst in checkCount.keys():
    checkEx[lst] = checkEx[lst]/checkCount[lst]
    checkSd[lst] = round(checkSd[lst]/checkCount[lst] - checkEx[lst]**2,4)**.5


for i in range(len(want)):
    for lst in checkCount.keys():
        if len(lst) == i+1:
            print lst, '\t',checkCount[lst],'\t',checkEx[lst],'\t',checkSd[lst],'\t',checkMn[lst],'\t',checkMx[lst]


#print excount

