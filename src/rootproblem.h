#ifndef ROOTPROBLEM_H
#define ROOTPROBLEM_H

#include <ilcplex/ilocplex.h>
#include <iostream>
#include "grid.h"
#include "tree.h"
#include "subproblem.h"
#include "scenarioproblem.h"
#include "rootresult.h"
#include "problemdata.h"


using namespace std;

class RootProblem {

	friend ostream& operator << (ostream& os, RootProblem& rp);

public:

	RootProblem(){}
	~RootProblem(){}

	void generateProblem();
	bool checkParameters(){ return _pd->checkParameters() ; }

	RootResult* solveProblem();
	RootResult* simProblem();
	void simProblemAndOutput(const char* name);

	void writeMPS(const char* name);

	void setProblemData(ProblemData* pd){_pd=pd;}

private:
	SubProblem _root;
	vector<ScenarioProblem*> _sc;
	
	ProblemData * _pd;

};

#endif
