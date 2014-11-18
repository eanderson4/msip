### A vector of data points

import operator
import os

class Cap:
    "A simple class"
    
    def __init__(self, filein, local):
        f = open( filein, 'r' )

        ls = f.readlines()
        
        for l in ls:
            l.split(', ')
            l
        
        self.lines = l.split(', ')

        self.names = [ '-1' ]
        self.ex = { (-1,-1):1 }
        if '' in self.lines:
            self.lines.remove('')

        if filein == 'allocate.lines':
            self.lines = set([ line.split('_')[0] for line in self.lines ])
        

        for line in self.lines:
            f = open(local+line+'.dat','r')
            ###Read in data file
            f.readline()        
            datain = f.readlines()
            for data in datain:
                run = data.split('\t')
#                print run
                self.names.append( line+'.'+ run[0])
                self.ex[ (line,run[0]) ] = run[2]
#            print self.ex
        self.names.pop(0)
    
    def params(self, lam, eta):
        self.lam = lam
        self.eta = eta


    def getNeighbors(self, line, point):
        less = [n for n in self.names if n.startswith(line+'.')  ]
        c = [t.split('.')[1]+'.'+t.split('.')[2] for t in less]
        print c
        lameff = self.lam * float(c[10])
        if '' in c:
            c.remove('')
        targets = [tg for tg in c if float(tg) >= (float(point) - lameff)*.9925 and float(tg) <= (float(point) + lameff)*(1.0075)]      
        return targets
        

    def isStable(self, line, point):
        nbs = self.getNeighbors( line, point)
        print nbs
        levels = [ float(self.ex[ line, tgt ]) for tgt in nbs ]
        print "Neighborhood Levels: ",levels
        print "Max Difference: ",max(levels)-min(levels)

        if max(levels)-min(levels) <= self.eta * float(point):
            return True
        else:
            return False
       

    def getStableSet(self):
        ss = [ -1 ]
        self.nbhd = { (-1,-1):1 }
        self.ratio = { (-1,-1):1 }
        for n in self.names:
#            print n
            line=n.split('.')[0]
            point=n.split('.')[1]+'.'+n.split('.')[2]
#            print line,point
            if self.isStable(line,point):
                self.nbhd[ (line,point) ] = float(self.ex[ (line,point) ]) - float( self.ex[ (line,'0.0') ])
                self.ratio[ (line,point) ] = self.nbhd[ (line,point) ]/float(point)
                ss.append(n)

        self.nbhd.pop( (-1,-1) )
        self.ratio.pop( (-1,-1) )
        ss.pop(0)
        return ss


    def getGreedySet(self,budget):
               
        sortedrat =  sorted( self.ratio.iteritems(), key=operator.itemgetter(1))
        print sortedrat[0:20]

        linestaken = [ '' ]
        picks = [ '' ]
        usedcap = 0
        for rat in sortedrat:
            
            capacity=float(rat[0][1])
            if rat[0][0] not in linestaken and usedcap+capacity<budget:
                picks.append( rat[0] )
                linestaken.append( rat[0][0] )
                usedcap = usedcap+capacity

        linestaken.pop(0)
        picks.pop(0)
        
        print linestaken
        print picks
                
        return picks

    def getGreedyPoint(self):
        print self.ex
        
        point=20000
        min_key=12

        if self.ex[(-1, -1)]:
            del self.ex[(-1, -1)]

        for key in self.ex.keys():
            print key, self.ex[key]
            value = float(self.ex[key])
            if value < point and value >= 0:
                point = value
                print 'YESYEYSY\n'
                min_key = key
        return min_key



    def createFiles(self,picks,budget):
        usedbudget = 0
        for p in picks:
            usedbudget = usedbudget + float(p[1])

        weights2 = [ (p,float(p[1])/usedbudget) for p in picks ]
        dwt = dict(weights2)
        print dwt

        print str(usedbudget)+" / "+str(budget)        
    

        count=1
        flist = open('allocate.lines','w')
        for p in picks:
            line = p[0]
            print line   
            basename='n'+str(count)
            count=count+1

            for i in range(31):
                name=basename+'_'+str(i)
                flist.write(name+", ")
                if not os.path.exists( name ):
                    os.mkdir( name )
                capfile = name+'/additions.cap'
                fout = open(capfile,'w')
                fout.write('Design Capacity -individual\n')

                w2 = float(i)/30
                w1 = 1-w2
                print w1,w2
                for w in dwt:
                    final = float(w[1])*w2
                    if w == p:
                        final = final + w1*float( p[1] )
                    fout.write(w[0] + ' ' + str(final)+'\n')
                    
                fout.close()
                print p
                print name       
                
        flist.close()
