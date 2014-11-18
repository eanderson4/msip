#ifndef SCENARIORESULT_H
#define SCENARIORESULT_H


#include <ilcplex/ilocplex.h>
#include "tree.h"
#include "problemdata.h"


using namespace std;

class ScenarioResult {

	friend ostream& operator << (ostream& os, ScenarioResult& r);
	
public:
	ScenarioResult(ProblemData* p);
	~ScenarioResult(){	}

	void setScenario(int i){ _scenario = i;}
        int getScenario(){ return _scenario; }

	void updateDTotal(int n, double value){ _dTotal[n]->setData(value); }
	void updateRTotal(int n, double value){ _rTotal[n]->setData(value); }
	void updatePDeltaTotal(int n, double value){ _pDeltaTotal[n]->setData(value); }
	void updateXTotal(int n, double value){ _xTotal[n]->setData(value); }
	void updateWTotal(int n, double value){ _wTotal[n]->setData(value); }
	void updateZTotal(int n, double value){ _zTotal[n]->setData(value); }
	void updateETotal(int n, double value){ _eTotal[n]->setData(value); }
	void updateDone(int n, double value){ _done[n]->setData(value); }
	void updateEDeltaPlus(int n, double value){ _eDeltaPlus[n]->setData(value); }
	void updateEDeltaMinus(int n, double value){ _eDeltaMinus[n]->setData(value); }

	void updatePDelta(int n,int m, double value){ _pDelta[n]->setDataV(m,value); }
	void updateX(int n,int m, double value){ _x[n]->setDataV(m,value); }
	void updateF(int n,int m, double value){  _f[n]->setDataV(m,value); }
	void updateP(int n,int m, double value){ _p[n]->setDataV(m,value); }
	void updateD(int n,int m, double value){ _d[n]->setDataV(m,value); }
	void updateE(int n,int m, double value){ _e[n]->setDataV(m,value); }
	
	
	double getDone(int n){ return _done[n]->getData(); }
	double getETotal(int n){ return _eTotal[n]->getData(); }
	double getDTotal(int n){ return _dTotal[n]->getData(); }
	double getXTotal(int n){ return _xTotal[n]->getData(); }

	double getF(int n, int j){ return _f[n]->getDataV(j); }
	double getP(int n, int j){ return _p[n]->getDataV(j); }
	double getPDelta(int n, int j){ return _pDelta[n]->getDataV(j); }
	double getD(int n, int j){ return _d[n]->getDataV(j); }
	double getX(int n, int j){ return _x[n]->getDataV(j); }
	double getE(int n, int j){ return _e[n]->getDataV(j); }
//	double getEDeltaPlus(int n, int j){ return _eDeltaPlus[n]->getData(j); }
	//double getEDeltaMinus(int n, int j){ return _eDeltaMinus[n]->getData(j); }

private:

	ProblemData * _pd;

	int _scenario;

	Tree<NodeData> _dTotal;
	Tree<NodeData> _d;
	Tree<NodeData> _rTotal;
	Tree<NodeData> _pDeltaTotal;
	Tree<NodeData> _pDelta;
	Tree<NodeData> _p;
	Tree<NodeData> _xTotal;
	Tree<NodeData> _x;
	Tree<NodeData> _f;
	Tree<NodeData> _wTotal;
	Tree<NodeData> _zTotal;
	Tree<NodeData> _e;
	Tree<NodeData> _eTotal;
	Tree<NodeData> _eDeltaPlus;
	Tree<NodeData> _eDeltaMinus;
	
	Tree<NodeData> _done;

};

#endif
