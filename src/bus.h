#ifndef BUS_H
#define BUS_H

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Bus {
	
	friend ostream& operator << (ostream& os, Bus& b);

public:
	Bus(){}
	~Bus(){}

	void updateBusNumber(int busNumber){ _busNumber = busNumber; }
	void updateGenMin(double genMin){ _genMin = genMin; }
	void updateGenMax(double genMax){ _genMax = genMax; }
	void updateNominalDemand(double nominalDemand){ _nominalDemand = nominalDemand; }
	void updateName(string name){ _name = name; }
//	void updateConnectedIndex(int connectedIndex){ _connectedIndex.push_back(connectedIndex); }
	void updateConnectedIn(int connectedIn){ _connectedIn.push_back(connectedIn); }
	void updateConnectedOut(int connectedOut){ _connectedOut.push_back(connectedOut); }


	int getBusNumber(){return _busNumber;}
	double getGenMin(){return _genMin;}
	double getGenMax(){return _genMax;}
	double getNominalDemand(){return _nominalDemand;}
//	vector<int> getConnectedIndex(){return _connectedIndex;}
	vector<int> getConnectedIn(){return _connectedIn; }
	vector<int> getConnectedOut(){return _connectedOut; }

private:
	int _busNumber;
	double _genMin, _genMax, _nominalDemand;
	string _name;
//	vector<int> _connectedIndex;
	vector<int> _connectedIn; 
	vector<int> _connectedOut;
};

#endif
