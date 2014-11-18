#!/progs/gurobi/linux64/bin/python2.7

import sys

def runInit():
    print "Initialize routine"


    #INTIIALIZEatION routine






if __name__=="__main__":

    if len(sys.argv) > 1:
        #FIND AWARENESS
        flog = open(sys.argv[1],'r')
        log = flog.readlines()
        flog.close()
        args = log[-1].split(" ")[1:]
        runit_args = [ arg.split("\n")[0] for arg in args ]
#        print runit_args

        #TIME TO PROCESS

        if 'start' in runit_args:
            runInit()
            #Create folders
            #mkdag
            fcon = open(sys.argv[2],'r')
            con = fcon.readlines()
            fcon.close()
            jobsrunning= con[-1].split(";")[1].split(", ")[3]
            jobsidle= con[-1].split(";")[1].split(", ")[2]
            jobsheld= con[-1].split(";")[1].split(", ")[4]
            sumo = int(jobsidle.split(" ")[0]) + int(jobsrunning.split(" ")[0]) + int(jobsheld.split(" ")[0])
            if sumo == 0:
                fout = open(sys.argv[1],'a')
                fout.write('-notrunning\n')
                fout.close()


        
        if 'check_queue' in runit_args:
            fcon = open(sys.argv[2],'r')
            con = fcon.readlines()
            fcon.close()
            jobsrunning= con[-1].split(";")[1].split(", ")[3]
            jobsidle= con[-1].split(";")[1].split(", ")[2]
            jobsheld= con[-1].split(";")[1].split(", ")[4]
            print jobsidle,jobsrunning,jobsheld
            sumo = int(jobsidle.split(" ")[0]) + int(jobsrunning.split(" ")[0]) + int(jobsheld.split(" ")[0])
            if sumo == 0:
                fout = open(sys.argv[1],'a')
                fout.write('-dosomething\n')
                fout.close()
            
        if 'iteration' in runit_args:
            fcheck = open(sys.argv[2],'r')
            check = fcheck.readlines()
            fcheck.close()

            fout = open(sys.argv[1],'a')
            maxim = 0
            if check:
                for c in check:
                    print c.split('break')[1].split('\n')[0]
                    test = int(c.split('break')[1].split('\n')[0])
                    if test > maxim:
                        maxim=test
#                fout.write(check[-1].split('break')[1].split('\n')[0])
                fout.write(str(maxim))
            else:
                print "HERE"
                print check
                fout.write( str(-1) )
    
            fout.close()


    else:
        "Usage: Proc.py <log input> <condor_q in>"
