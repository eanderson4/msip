#ifndef ROOTRESULT_H
#define ROOTRESULT_H

#include <vector>
#include "scenarioresult.h"

using namespace std;

class RootResult {

	friend ostream& operator << (ostream& os, RootResult& r);

public:
	RootResult(ProblemData* p, bool sim=false);
	~RootResult(){	}

	void outputDesignCapacity(const char*);
	void outputFinalDemand(const char*);
	void outputLineOutages(const char*);
	
	double getMipGap(){ return _mipGap; }

	void updateSR(ScenarioResult* sr){ _sr.push_back(sr); }
	void updateGap(double d){ _mipGap = d; }
	void updateDesignCapacity(double d){ _designCapacity = d; }
	void updateDTotal(int scenario, int n, double value){ _sr[scenario]->updateDTotal(n,value); }
	void updateRTotal(int scenario, int n, double value){ _sr[scenario]->updateRTotal(n,value); }
	void updatePDeltaTotal(int scenario, int n, double value){ _sr[scenario]->updatePDeltaTotal(n,value); }
	void updateXTotal(int scenario, int n, double value){ _sr[scenario]->updateXTotal(n,value); }
	void updateWTotal(int scenario, int n, double value){ _sr[scenario]->updateWTotal(n,value); }
	void updateZTotal(int scenario, int n, double value){ _sr[scenario]->updateZTotal(n,value); }
	void updateX(int scenario, int n, int m, double value){ _sr[scenario]->updateX(n,m,value); }

	void addedCapacity(double d){ _addedCapacity.push_back(d); }


private:

	vector<ScenarioResult*> _sr;

	double _mipGap;
	double _designCapacity;

	vector<double> _addedCapacity;

	ProblemData * _pd;

};

#endif
