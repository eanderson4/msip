#!/usr/bin/python
line1=67
line2=79

finfo = open("info")
infolines = finfo.readlines()

it = float(infolines[0].split(" ")[1].rstrip())
delta = float(infolines[1].split(" ")[1].rstrip())
deltatol = float(infolines[2].split(" ")[1].rstrip())
z = float(infolines[3].split("  ")[3].rstrip())

f = open("add.cap")

lines =f.readlines()

x1 = float(lines[1].split(" ")[1].rstrip())
x2 = float(lines[2].split(" ")[1].rstrip())

print x1,x2
print it,z
print delta,deltatol


t11=x1+delta
t12=x2
fout1 = open("t1.cap",'w')
fout1.write("Design Capacity -individual\n")
fout1.write(str(line1)+" "+str(t11)+"\n")
fout1.write(str(line2)+" "+str(t12)+"\n")

t21=x1-delta
t22=x2
fout2 = open("t2.cap",'w')
fout2.write("Design Capacity -individual\n")
fout2.write(str(line1)+" "+str(t21)+"\n")
fout2.write(str(line2)+" "+str(t22)+"\n")

t31=x1
t32=x2+delta
fout3 = open("t3.cap",'w')
fout3.write("Design Capacity -individual\n")
fout3.write(str(line1)+" "+str(t31)+"\n")
fout3.write(str(line2)+" "+str(t32)+"\n")

t41=x1
t42=x2-delta
fout4 = open("t4.cap",'w')
fout4.write("Design Capacity -individual\n")
fout4.write(str(line1)+" "+str(t41)+"\n")
fout4.write(str(line2)+" "+str(t42)+"\n")
