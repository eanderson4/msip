#!/usr/bin/python
line1=67
line2=79

finfo = open("info")
infolines = finfo.readlines()
finfo.close()

it = float(infolines[0].split(" ")[1].rstrip())
delta = float(infolines[1].split(" ")[1].rstrip())
deltatol = float(infolines[2].split(" ")[1].rstrip())
#z = float(infolines[3].split("  ")[3].rstrip())

f = open("add.cap")

lines =f.readlines()

x1 = float(lines[1].split(" ")[1].rstrip())
x2 = float(lines[2].split(" ")[1].rstrip())

#print x1,x2
#print it,z
#print delta,deltatol


print "1"


foutfo=open("info",'w')
it=it+1
delta=delta
deltatol=deltatol
print "iteration",it
print "delta",delta
print "deltatol",deltatol

