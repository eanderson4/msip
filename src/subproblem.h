#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include <iostream>
#include <ilcplex/ilocplex.h>
#include <sstream>
#include "node.h"
#include "grid.h"
#include "problemdata.h"

using namespace std;

extern double g_AMAX;

class SubProblem : public Node {

	friend ostream& operator << (ostream& os, SubProblem& sp);

public:
	SubProblem():Node(){}
	SubProblem(int n,int stage,int parent):Node(n,stage,parent){ }
	~SubProblem(){}

	void generateSubProblem(int,ProblemData*, int, SubProblem* s=0); //fill constraints 
	void setOutage(int, bool);
	void setPriorES(int n , double d){ 
			_ePrior[n] = d; 
			_energyStorage[n].setExpr(  _e[n] - _ePrior[n] + _eDeltaPlus[n] - _eDeltaMinus[n]);
	} 


	double effectiveCapacity(double);//get effective capacity from inverse distribution
	
	char * getName(const char * prefix,int node, int bus, char * buffer);

	IloNumVarArray getF(){ return _f; }
	IloNumVarArray getP(){ return _p; }
	IloNumVarArray getR(){ return _r; }
	IloNumVarArray getPDeltaPlus(){ return _pDeltaPlus; }
	IloNumVarArray getPDeltaMinus(){ return _pDeltaMinus; }
	IloNumVarArray getD(){ return _d; }
	IloNumVarArray getTheta(){ return _theta; }
	IloNumVarArray getE(){ return _e; }
	IloNumVarArray getEDeltaPlus(){ return _eDeltaPlus; }
	IloNumVarArray getEDeltaMinus(){ return _eDeltaMinus; }

	IloNumArray	getEffectiveCapacity(){ return _effectiveCapacity; }
	IloNumArray	getEPrior(){ return _ePrior; }

	IloBoolVarArray getZ(){ return _z; }
	IloBoolVarArray getW(){ return _w; }
	IloBoolVarArray getX(){ return _x; }
	IloBoolVarArray getY(){ return _y; }

	IloRangeArray getEnergyConservation(){ return _energyConservation; }
	IloRangeArray getPhaseAngleUp(){ return _phaseAngleUp; }
	IloRangeArray getPhaseAngleDown(){ return _phaseAngleDown; }
	IloRangeArray getLineLimitsUpM(){ return _lineLimitsUpM; }
	IloRangeArray getLineLimitsDownM(){ return _lineLimitsDownM; }
	IloRangeArray getLineLimitsUp(){ return _lineLimitsUp; }
	IloRangeArray getLineLimitsDown(){ return _lineLimitsDown; }
	IloRangeArray getLineSwitch(){ return _lineSwitch; }
	IloRangeArray getLineBurnt(){ return _lineBurnt; }
	IloRangeArray getGenLimitsUp(){ return _genLimitsUp; }
	IloRangeArray getGenLimitsDown(){ return _genLimitsDown; }
	IloRangeArray getReserveMargin(){ return _reserveMargin; }
	IloRangeArray getRampingCapacity(){ return _rampingConstraint; }
	
	IloRangeArray getGenReDispatch(){ return _genReDispatch; }
	IloRangeArray getEffectiveCapacityUp(){ return _effectiveCapacityUp; }
	IloRangeArray getEffectiveCapacityDown(){ return _effectiveCapacityDown; }
	IloRangeArray getBurntOut(){ return _burntOut; }
	IloRangeArray getDemandRelation(){ return _demandRelation; }
	IloRangeArray getEnergyStorage(){ return _energyStorage; }

	IloExpr getSumD(){ return IloSum(_d); }
	IloExpr getSumP(){ return IloSum(_p); }
	IloExpr getSumR(){ return IloSum(_r); }
	IloExpr getSumPDeltaPlus(){ return IloSum(_pDeltaPlus); }
	IloExpr getSumPDeltaMinus(){ return IloSum(_pDeltaMinus); }
	IloExpr getSumX(){ return IloSum(_x); }
	IloExpr getSumW(){ return IloSum(_w); }
	IloExpr getSumZ(){ return IloSum(_z); }


private:

	ProblemData* _pd;
	int _scenario;

	IloEnv _env;

	IloNumVarArray _f;	//flow on branches
	IloNumVarArray _p;	//generator dispatch
	IloNumVarArray _r;  //generator reserve
	IloNumVarArray _pDeltaPlus; //generator reDispatch
	IloNumVarArray _pDeltaMinus; //generator reDispatch
	IloNumVarArray _d;	//demand served
	IloNumVarArray _theta;	//phase angle of nodes
	IloNumVarArray _e;	//energy stored
	IloNumVarArray _eDeltaPlus;	//change in energy storage
	IloNumVarArray _eDeltaMinus;	//change in energy storage
	IloNumArray _ePrior;	//change in energy storage

	IloBoolVarArray _z;	//indicator for line failed, fail if either x or w
	IloBoolVarArray _w;	//indicator for line switched
	IloBoolVarArray _x;	//indicator for line burned out
	IloBoolVarArray _y;	//indicator for generator on/of

	IloRangeArray _energyConservation; //energy conservation at buses
	IloRangeArray _genLimitsUp;	//limits on generator if on/off
	IloRangeArray _genLimitsDown;	//limits on generator if on/off
	IloRangeArray _reserveMargin;	//Limits total reserve available


	IloRangeArray _phaseAngleUp;	//branch flow relation with relaxation based on line out
	IloRangeArray _phaseAngleDown;	//branch flow relation with relaxation based on line out
	IloRangeArray _lineLimitsUpM;	//BIGM line limits with relaxation based on line out
	IloRangeArray _lineLimitsDownM;	//BIGM line limits with relaxation based on line out
	IloRangeArray _lineLimitsUp;	//line limits with relaxation based on line out
	IloRangeArray _lineLimitsDown;	//line limits with relaxation based on line out
	IloRangeArray _lineSwitch;	//for line switch
	IloRangeArray _lineBurnt;	//for lines burning out

	//stage linking constraints
	IloRangeArray _genReDispatch;
	IloRangeArray _effectiveCapacityUp;	//chance constraint for shaping distribution of line outages
	IloRangeArray _effectiveCapacityDown;	//chance constraint for shaping distribution of line outages
	IloRangeArray _burntOut;		//if a line is over effective capacity, it burns out
	IloRangeArray _demandRelation;	//prevents lp cheating
	IloRangeArray _rampingConstraint;	//Ramping constraint, based on reserve capacity
	IloRangeArray _energyStorage; //energy conservation at buses

	SubProblem* _parent;

	IloNumArray _uZeroOne;
	IloNumArray _effectiveCapacity;

};

#endif
