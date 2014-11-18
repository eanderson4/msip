#ifndef GRID_H
#define GRID_H

#include <ilcplex/ilocplex.h>
#include <exception> 
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "branch.h"
#include "bus.h"


class FileNotRead: public exception
{
  virtual const char* what() const throw()
  {
    return "File Not Read";
  }
};

using namespace std;

class Grid {

	friend ostream& operator << (ostream& os, Grid& g);

public:
	Grid(){}
	~Grid(){}

	void readInFile(const char * fileName);
	void setBranchCapacity(int n, double d){
	  _branches[n].updateCapacity(d);
	}

	int getID(){ return _ID; }		
	int getBusSize(){ return _buses.size(); }			
	int getBranchSize(){ return _branches.size(); }

	int getSendIndex(int n){ return _branches[n].getSendIndex(); }		
	int getRecIndex(int n){ return _branches[n].getRecIndex(); }
	double getBranchX(int n){ return _branches[n].getX(); }
	double getBranchCapacity(int n){ return _branches[n].getCapacity(); }
	double getBranchStatus(int n){ return _branches[n].getStatus(); }

	int getBusNumber(int n){ return _buses[n].getBusNumber(); }
	double getGenMax(int n){ return _buses[n].getGenMax(); }
	double getGenMin(int n){ return _buses[n].getGenMin(); }
	double getNominalDemand(int n){ return _buses[n].getNominalDemand(); }

	vector<int> getConnectedIn(int n){ return _buses[n].getConnectedIn(); }
	vector<int> getConnectedOut(int n){ return _buses[n].getConnectedOut(); }


private:

	int _ID;

	vector<Branch> _branches;
	vector<Bus> _buses;

};

#endif

