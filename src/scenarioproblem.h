#ifndef SCENARIOPROBLEM_H
#define SCENARIOPROBLEM_H

#include <ilcplex/ilocplex.h>
#include <iostream>
#include "grid.h"
#include "tree.h"
#include "subproblem.h"
#include "scenarioresult.h"


using namespace std;

class ScenarioProblem {

	friend ostream& operator << (ostream& os, ScenarioProblem& mp);

public:

	ScenarioProblem(){_pd=0;_scenario=0;}
	~ScenarioProblem(){}

	void buildTree();

	void setScenario(int i){ _scenario = i; }
	void setTrial(int i){ _trial = i; }
	void setProblemData(ProblemData* pd){ _pd = pd; }

	void generateProblem();	//generato problem from grid information
	void addSubProblem(int);	//add subproblem from vector to model
	
	ScenarioResult * simProblem();
	void addSimProblem(IloModel);
	
	SubProblem* getSubProblem(int i){ return _sp[i]; }
	int getSize(){ return _sp.getSize(); }
	IloExpr getWeightedDemand(){ return _sumWeightedDemand; }
	

private:
	Tree<SubProblem> _sp;
	SubProblem _simSP;

	IloExpr _sumWeightedDemand;

	int _scenario;
	int _trial;
	ProblemData * _pd;


};

#endif
