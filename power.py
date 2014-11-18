#Power System Tools

class Bus:
    """Stores bus information"""
    def __init__(self, ID, name, pmin, pmax):
        self.ID = ID
        self.name = name
        self.pmin = pmin
        self.pmax = pmax
    def __str__(self):
        return self.ID + ": " + self.name + "  ( "+ self.pmin +" , "+ self.pmax + " ) "

class Branch:
    """Stores branch information"""
    def __init__(self, ID, bus1, bus2, limit, X):
        self.ID = ID
        self.bus1 = bus1
        self.bus2 = bus2
        self.limit = limit
        self.X = X

    def __str__(self):
        return self.ID + ": " + self.bus1 +" -> " +self.bus2 + " , " +self.limit+ " ("+self.X+")"

class Grid:
    """Stores grid information"""
    def printBranch(self, ID):
        print  self.branches[ ID ]
    def printBus(self, ID):
        print  self.buses[ ID ]

    def getBranchLimit(self, ID):
        return self.branches[ ID ].limit

    def getNumBranch(self):
        return self.Nbranch

    def readData(self,fin):
        f = open(fin,'r')
        l = f.readline()
        while l:
            if 'GRIDDATA' in l:
                print 'Grid data ID: #Buses #Branches'
                #read in grid information
                l = f.readline()
                temp = l.split(" ")
                self.ID=temp[1]
                self.Nbus=temp[2]
                self.Nbranch=temp[3].split("\r")[0]
                print self.ID,": ",self.Nbus, self.Nbranch
#                print "Grid ",self.ID,": ",self.Nbus, self.Nbranch

                
            if 'BUSDATA' in l:
                print 'Bus data  ID: name  (pmin, pmax)'
                #read in bus information and store in dictionary
                l = f.readline()
                temp = l.split(" ")
                b = Bus(-1,0,0,0)
                self.buses = { b.ID : b }
                while temp[0]=='d':
                    #parse bus string
                    bus = Bus(temp[1], temp[3], temp[4], temp[5].split("\r")[0] )
                    self.buses[ bus.ID ] = bus
                    print bus
                    temp = f.readline().split(" ")               


            if 'BRANCHDATA' in l:
                print 'Branch data  ID: bus1 -> bus2 , capacity (X)'
                #read in branch information
                l = f.readline()
                temp = l.split(" ")
                b = Branch( -1, 0, 0, 0, 0 )
                self.branches = { b.ID : b}
                while temp[0]=='d':
                    #parse branch string
                    branch = Branch(temp[1], temp[2], temp[3], temp[4], temp[5].split("\r")[0] )
                    self.branches[branch.ID] = branch
                    print branch
                    temp = f.readline().split(" ")


            l = f.readline()

        print 'File read in complete'
        f.close()
            

    def __init__(self):
        self.data = []


