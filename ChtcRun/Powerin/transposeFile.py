#!/progs/gurobi/linux64/bin/python2.7

import sys

if __name__=="__main__":


    if len(sys.argv) > 2:

        print sys.argv

        filelocation = sys.argv[2]

        linesin = sys.argv[1]
        fl = open( linesin, 'r')
        lines = fl.readline().split(', ')
        if '' in lines:
            lines.remove('')
        lines = set([ l.split('_')[0] for l in lines ] )
        print lines

        for line in lines:
            cdatain = filelocation + line + '.cdat'
            cdataout = filelocation + line + '.ccdat'
            fin = open( cdatain,'r')
            fout = open( cdataout,'w') 
            cdata = fin.readlines()
            rows = [ line.split('\t') for line in cdata ]
            columns = zip(*rows)

            print columns[0]
            print rows[0]

            start = True
            for col in columns:
                if not start:
                    fout.write('\n')
                if start:
                    start = False
                for c in col:
                    fout.write(c.split('\n')[0]+'\t')
            fin.close()
            fout.close()
                    
                    

    else:
        print "Usage:  transposeFile.py <line file list>"
