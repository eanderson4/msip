#!/progs/gurobi/linux64/bin/python2.7

import sys
from tools import *

def makeCondorSub(cap1,cap2):
    filename = 'con_2d.sub'
    fout = open(filename,'w')

    print "Write to " + filename
    
    for c1 in cap1:
        for c2 in cap2:
            name = 'a'+str(c1)+'b'+str(c2)
            outname = 'condor/'+name+'.out'
            logname = 'condor/con_2d.log'
            capfile = 'input/'+name+'.cap'
            fout.write('universe = vanilla\n')
#            fout.write('executable = /data/eanderson/root/power/msip \n')
            fout.write('executable = /mnt/ws/opt/home/eanderson/root/power/msip \n')
            fout.write('arguments = input/big_sim_cplex.in '+capfile+' '+name+' \n')
            fout.write('output = '+outname+' \n')
            fout.write('log = '+logname+' \n')
            fout.write('+Group = "WID" \n')
#            fout.write('+MachineOwner = "Optimization" \n')
            fout.write('request_cpus = 1\n')
            fout.write('request_memory = 3000\n')
            fout.write('request_disk =25000\n')
            fout.write('notification = never\n')
            fout.write('queue\n')
    fout.close()

def makeCapFile(cap1,cap2):

    for c1 in cap1:
        for c2 in cap2:
            capfile = 'input/a'+str(c1)+'b'+str(c2)+'.cap'
            fout = open(capfile,'w')
            
            fout.write('Design Capacity -individual\n')
            fout.write('115 '+str(c1)+'\n')
            fout.write('133 '+str(c2))
        fout.close()

def process(cap1,cap2):
    for c1 in cap1:
        for c2 in cap2:
            print str(c1) +" , "+str(c2)
            name = 'tmp/a'+str(c1)+'b'+str(c2)
            loadshedanalysis(name+'.dem',name+'.lsa',3668)

def outputData(cap1,cap2):
    f = open('con_2d.dat','w')
    f.write('run smp ex sd se min max var cvar\n')
    for c1 in cap1:
        for c2 in cap2:
            name = 'a'+str(c1)+'b'+str(c2)
            ts = readlsa('tmp/'+name+'.lsa')
            f.write(name+'\t')
            for t in ts:
                f.write(t+'\t')
            f.write('\n')
    f.close()
            
def countLines(cap1,cap2):
    for c1 in cap1:
        for c2 in cap2:
            name = 'a'+str(c1)+'b'+str(c2)
            print "Counting "+name
            count( 'tmp/'+name+'.lin', 'tmp/'+name+'.lao', 10000 )

if __name__=="__main__":


    if len(sys.argv) > 1:
        cap1 = [0, 1,2,3,4,5,6,7,8,9, 10,11,12,13,14,15, 300 ]
        cap2 = [0, 1,2,3,4, 5, 500 ]
#        cap2 = [ 0 ]
#    cap2 = [0, 500, 1000,2500]

        if sys.argv[1] == '-p':
            print "Process"
            process(cap1,cap2)
            outputData(cap1,cap2)
        if sys.argv[1] == '-c':
            print "Make condor submit files"
            makeCapFile(cap1,cap2)
            makeCondorSub(cap1,cap2)
        if sys.argv[1] == '-l':
            print "Count line outages"
            countLines(cap1,cap2)

    else:
        print "Usage: consub.py <operation>  (Interface with condor)"
        print "            -p process files"
        print "            -c make condor submit"
        print "            -l count line outages"



    


    
    
    

