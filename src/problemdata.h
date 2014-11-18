#ifndef PROBLEMDATA_H
#define PROBLEMDATA_H

#include <ilcplex/ilocplex.h>
#include <iostream>
#include "grid.h"


using namespace std;

class ProblemData {

	friend ostream& operator << (ostream& os, ProblemData& pd);

public:

	ProblemData(){
	  _g=0; _stages=3;_scenarios=0;_trials=1;
	  int outcomes[3]={3, 3, 3};_outcomes = outcomes;_reserveMargin=.1; 
	  _allowGeneratorSwitch=false;_allowLineSwitch=false;
	  _allowReDispatch=true; _allowReserve=false;
	  _allowCapacity=false;_simProblem=false;
	  _subProblemGenerated=false;
	  _effectiveDistributionL=.5;  
	  _maxOutages=10;  _model = IloModel(_env); 
	  _stream = &cout; _allowEnergyStorage=false;
	  _maxRateES=0;_maxCapacityES=0;_initialChargeES=0;
	  _solLimit=0; _timeLimit=0; _consoleOutput=true;
	}
	~ProblemData(){}

	void readInData(const char*);
	void readInCapAdd(const char*);

	bool checkParameters();
	void designCapacity(double,double, double, int);
	void fixCapacity(double);
	void fixLine(int i, double d){ 
			_designCapacity[i-1].setBounds(d, d);
			_maxCapacityPerLine=d;
	}
	void setSubProblemGenerated(bool spg){ _subProblemGenerated=spg;}
       void setEnv(IloEnv e){ _env=e; }

	char * getName(const char * prefix,int bus, char * buffer);


	void setObjective(IloObjective o){ _obj = o; 
			_model.add(_obj);}

	bool getConsoleOutput(){ return _consoleOutput;}
	bool getSubProblemGenerated(){ return _subProblemGenerated;}
	double getGapLimit(){ return _gapLimit;}
	int getTimeLimit(){ return _timeLimit;}
	int getSolLimit(){ return _solLimit;}
	bool getMPS(){ return _solveMPS;}
	bool getCPLEX(){ return _solveCPLEX;}
	bool getSIM(){ return _simProblem;}
	bool getMSIP(){ return _msipProblem;}
	const char * getOutputName(){ return _outputFile.c_str();}
	const char * getCustomName(const char*);
	Grid * getGrid(){ return _g; }
	IloEnv getEnv(){ return _env; }
	IloModel getModel(){ return _model; }
	IloNumVarArray getDesignCapacity(){ return _designCapacity; }
	int getStages(){ return _stages; }
	int * getOutcomes(){ return _outcomes; }
	int getScenarios(){ return _scenarios; }
	int getTrials(){ return _trials; }
	int getNumberOutages(int scene){ return _outages[scene].size(); }
	int getOutages(int scene, int index){ return _outages[scene][index]; }
	int getMaxLineChanges(){ return _maxLineChanges; }
	bool getGeneratorSwitch(){ return _allowGeneratorSwitch; }
	bool getLineSwitch(){ return _allowLineSwitch; }
	bool getReDispatch(){ return _allowReDispatch; }
	bool getPenaltyDispatch(){ return _allowPenaltyDispatch; }
	double getPenaltyWeight(){ return _penaltyWeight; }
	bool getReserve(){ return _allowReserve; }
	bool getCapacity(){ return _allowCapacity; }
	bool getEnergyStorage(){ return _allowEnergyStorage; }
	bool getSimProblem(){ return _simProblem; }
	bool getLineOutages(){ return _outputLineOutages; }
	double getMaxCapacity(){ return _maxCapacity; }
	double getMaxCapacityPerLine(){ return _maxCapacityPerLine; }
	double getReserveMargin(){ return _reserveMargin; }
	double getObjWeights(int n){ return _objWeights[n]; }
	double getEffectiveDistribution(){ return _effectiveDistributionL; }
	double getProbability(){ return _probability; }
	double getMaxRateES(){ return _maxRateES; }
	double getMaxCapacityES(){ return _maxCapacityES; }
	double getInitialChargeES(){ return _initialChargeES; }
	int getSeed(){ return _seed; }
	ostream * getStream(){ return _stream; }


private:

	IloEnv _env;
	IloModel _model;
	IloObjective _obj;

	//linking variables
	IloNumVarArray _designCapacity;
	IloBoolVarArray _designLines;
	IloRangeArray _budgetLinesUp;
	IloRangeArray _budgetLinesDown;
	IloRange _budgetCapacity;
	IloRange _budgetLines;

	double _minCapacityPerLine;
	double _maxCapacityPerLine;
	double _maxCapacity;
	int _maxLineChanges;
	bool _allowCapacity;
	bool _subProblemGenerated;

	Grid* _g;

	int _stages;
	int * _outcomes;
	int _scenarios;
	int _trials;

	int _maxOutages;
	vector< vector<int> > _outages;

	int _seed;

	bool _allowGeneratorSwitch;
	bool _allowLineSwitch;
	bool _allowReDispatch;
	bool _allowReserve;
	
	bool _allowPenaltyDispatch;
	double _penaltyWeight;

	double _maxRateES;
	double _maxCapacityES;
	double _initialChargeES;
	bool _allowEnergyStorage;

	bool _simProblem;
	bool _msipProblem;

	bool _solveMPS;
	bool _solveCPLEX;

	bool _consoleOutput;
	bool _outputLineOutages;
	
	int _timeLimit;
	int _solLimit;
	double _gapLimit;

	double _reserveMargin;
	vector<double> _objWeights;
	double _effectiveDistributionL;
	double _probability;

	ostream * _stream;
	string _outputFile;
	string _name; 
};

#endif
