#include "subproblem.h"

double g_AMAX=360;	//max angle


void SubProblem::generateSubProblem(int node, ProblemData* pd, int scenario, SubProblem* s){
	//fill arrays with actual variables and constraints
	//with parameters from grid
	

	_pd = pd;
	_pd->setSubProblemGenerated(true);
	_env=_pd->getEnv();
	Grid* g = _pd->getGrid();
	_parent = s;

	_scenario=scenario;

	int V = g->getBusSize();	//number vertices
	int E = g->getBranchSize();  //number edges
	double bigM;


	int seed = _pd->getSeed() + scenario*100000;
	int subSeed = seed + node*1000;


	char buffer[32];

	//generate bus variables
	_p = IloNumVarArray(_env,V,0,IloInfinity);
	_d = IloNumVarArray(_env,V,0,IloInfinity);
	_theta = IloNumVarArray(_env,V,-g_AMAX,g_AMAX);
	if(_pd->getReserve()){
		_r = IloNumVarArray(_env,V,0,IloInfinity);
		_reserveMargin = IloRangeArray(_env, 1, -IloInfinity, 0);
		_reserveMargin[0].setExpr( (1-_pd->getReserveMargin())*getSumR() - _pd->getReserveMargin()*getSumP() );
		_reserveMargin[0].setName( getName("rm",0,node,buffer) );
	}
	if(_pd->getGeneratorSwitch()){
		_y = IloBoolVarArray(_env,V);
	}
	
	if(_pd->getEnergyStorage()){
		_e = IloNumVarArray(_env,V,0,_pd->getMaxCapacityES() );
		_eDeltaPlus = IloNumVarArray(_env,V,0, _pd->getMaxRateES());
		_eDeltaMinus = IloNumVarArray(_env,V, 0, _pd->getMaxRateES());
		_ePrior = IloNumArray(_env,V);
		for(int i=0;i<V;i++){ 
			_ePrior[i] = _pd->getInitialChargeES()*_pd->getMaxCapacityES(); 
		}
		_energyStorage = IloRangeArray(_env, V, 0, 0);
	}
	
	//generate branch variables
	_f = IloNumVarArray(_env,E,-IloInfinity,IloInfinity);
	_x = IloBoolVarArray(_env,E);
	if(_pd->getLineSwitch()){
		_z = IloBoolVarArray(_env,E);
		_w = IloBoolVarArray(_env,E);
	}

	//generate constraints on bus
	_energyConservation = IloRangeArray(_env, V, 0, 0);
	_genLimitsUp = IloRangeArray(_env, V, -IloInfinity, 0);
	_genLimitsDown = IloRangeArray(_env, V, 0, IloInfinity);
	

	vector<int> conIn;
	vector<int> conOut;


	for(int i=0;i<V;i++){
		IloExpr tempEC(_env);
		conIn = g->getConnectedIn(i);
		conOut = g->getConnectedOut(i);
		tempEC+= _p[i];
		tempEC-= _d[i];
		
		if(_pd->getEnergyStorage() ){
			tempEC+=_eDeltaPlus[i] - _eDeltaMinus[i];
		}

		for(int j=0;j<(int)conIn.size();j++){
			tempEC-=_f[conIn[j]];
		}
		for(int j=0;j<(int)conOut.size();j++){
			tempEC+=_f[conOut[j]];
		}
		_energyConservation[i].setExpr(tempEC);
		tempEC.end();
		
		if(_pd->getEnergyStorage()){

			if(_parent==0){
				_energyStorage[i].setExpr(  _e[i] - _ePrior[i] + _eDeltaPlus[i] - _eDeltaMinus[i] );
			}
			else{
				_energyStorage[i].setExpr(  _e[i] - _parent->getE()[i] - _eDeltaPlus[i] + _eDeltaMinus[i] );
			}

		}

		if(_pd->getReserve()){
			if(_pd->getGeneratorSwitch()){
				_genLimitsUp[i].setExpr( _p[i] + _r[i] - g->getGenMax(i)*_y[i] );
				_genLimitsDown[i].setExpr( _p[i] + _r[i] - g->getGenMin(i)*_y[i] );
				_y[i].setName( getName("y",i,node,buffer) );
			}
			else{
				_genLimitsUp[i].setExpr( _p[i] + _r[i] - g->getGenMax(i) );
				_genLimitsDown[i].setExpr( _p[i] + _r[i] - g->getGenMin(i) );
			}
		}
		else{
			if(_pd->getGeneratorSwitch()){
				_genLimitsUp[i].setExpr( _p[i] - g->getGenMax(i)*_y[i] );
				_genLimitsDown[i].setExpr( _p[i] - g->getGenMin(i)*_y[i] );
				_y[i].setName( getName("y",i,node,buffer) );
			}
			else{
				_genLimitsUp[i].setExpr( _p[i] - g->getGenMax(i) );
				_genLimitsDown[i].setExpr( _p[i] - g->getGenMin(i) );
			}
		}

		_d[i].setUB(g->getNominalDemand(i));

		//Name Variables and Constraints
		_p[i].setName( getName("p",i,node,buffer) );
		_d[i].setName( getName("d",i,node,buffer) );
		if(_pd->getReserve()) _r[i].setName( getName("r",i,node,buffer) );
		if(_pd->getEnergyStorage()){
			_e[i].setName( getName("e",i,node,buffer) );
			_eDeltaPlus[i].setName( getName("eDeltaPlus",i,node,buffer) );
			_eDeltaMinus[i].setName( getName("eDeltaMinus",i,node,buffer) );
			_energyStorage[i].setName( getName("es",i,node,buffer) );
		}
		_theta[i].setName( getName("theta",i,node,buffer) );

		_energyConservation[i].setName( getName("ec",i,node,buffer) );
		_genLimitsUp[i].setName( getName("glu",i,node,buffer) );
		_genLimitsDown[i].setName( getName("gld",i,node,buffer) );
	}

	//generate constraint on branch
	_phaseAngleUp = IloRangeArray(_env,E,-IloInfinity,0);
	_phaseAngleDown = IloRangeArray(_env,E,0,IloInfinity);
	_lineLimitsUp = IloRangeArray(_env,E,-IloInfinity,0);
	_lineLimitsDown = IloRangeArray(_env,E,0,IloInfinity);
	if(_pd->getCapacity()){
	_lineLimitsUpM = IloRangeArray(_env,E,-IloInfinity,0);
	_lineLimitsDownM = IloRangeArray(_env,E,0,IloInfinity);
	}
	if(_pd->getLineSwitch()){
		_lineSwitch = IloRangeArray(_env,E,-1,0);
		_lineBurnt = IloRangeArray(_env,E,-1,0);
	}
	
	_effectiveCapacity = IloNumArray(_env,E);

	int send;
	int rec;
	double X;
	double maxPossibleCapacity;

	for(int i=0;i<E;i++){
		send = g->getSendIndex(i);
		rec = g->getRecIndex(i);
		X = g->getBranchX(i);
		
		if(_pd->getCapacity()) maxPossibleCapacity = g->getBranchCapacity(i) + _pd->getMaxCapacityPerLine();
		else maxPossibleCapacity = g->getBranchCapacity(i);
		
		bigM = 2*g_AMAX + X*(maxPossibleCapacity);

		if(_pd->getLineSwitch()){		
			_phaseAngleUp[i].setExpr(_theta[send] - _theta[rec] + X*_f[i] - bigM*(1-_z[i]));
			_phaseAngleDown[i].setExpr(_theta[send] - _theta[rec] + X*_f[i] + bigM*(1-_z[i]));
			
			if(_pd->getCapacity()){
				_lineLimitsUpM[i].setExpr(_f[i] - maxPossibleCapacity*_z[i]);
				_lineLimitsDownM[i].setExpr(_f[i] + maxPossibleCapacity*_z[i]);
				_lineLimitsUp[i].setExpr(_f[i] - g->getBranchCapacity(i) - _pd->getDesignCapacity()[i]);
				_lineLimitsDown[i].setExpr(_f[i] + g->getBranchCapacity(i) + _pd->getDesignCapacity()[i]);
			}
			else{	
				_lineLimitsUp[i].setExpr(_f[i] - g->getBranchCapacity(i)*_z[i]);
				_lineLimitsDown[i].setExpr(_f[i] + g->getBranchCapacity(i)*_z[i]);
			}
			
			_lineSwitch[i].setExpr(_z[i] + _w[i] -1);
			_lineBurnt[i].setExpr(_z[i] + _x[i] -1);

			_z[i].setName( getName("z",i,node,buffer) );
			_w[i].setName( getName("w",i,node,buffer) );
			_lineSwitch[i].setName( getName("ls",i,node,buffer) );
			_lineBurnt[i].setName( getName("lb",i,node,buffer) );
		}
		else{	
			_phaseAngleUp[i].setExpr(_theta[send] - _theta[rec] + X*_f[i] - bigM*_x[i]);
			_phaseAngleDown[i].setExpr(_theta[send] - _theta[rec] + X*_f[i] + bigM*_x[i]);

			if(_pd->getCapacity()){
				_lineLimitsUpM[i].setExpr(_f[i] - maxPossibleCapacity*(1-_x[i]));
				_lineLimitsDownM[i].setExpr(_f[i] + maxPossibleCapacity*(1-_x[i]));
				_lineLimitsUp[i].setExpr(_f[i] - g->getBranchCapacity(i) - _pd->getDesignCapacity()[i] );
				_lineLimitsDown[i].setExpr(_f[i] + g->getBranchCapacity(i) + _pd->getDesignCapacity()[i]);
			}
			else{
				_lineLimitsUp[i].setExpr(_f[i] - g->getBranchCapacity(i)*(1-_x[i]));
				_lineLimitsDown[i].setExpr(_f[i] + g->getBranchCapacity(i)*(1-_x[i]));
			}
		}
			

		//Name Variables and Constraints
		_f[i].setName( getName("f",i,node,buffer) );
		_x[i].setName( getName("x",i,node,buffer) );

		_phaseAngleUp[i].setName( getName("pau",i,node,buffer) );
		_phaseAngleDown[i].setName( getName("pad",i,node,buffer) );
		_lineLimitsUp[i].setName( getName("llu",i,node,buffer) );
		_lineLimitsDown[i].setName( getName("lld",i,node,buffer) );
		if(_pd->getCapacity()){
			_lineLimitsUpM[i].setName( getName("lluM",i,node,buffer) );
			_lineLimitsDownM[i].setName( getName("lldM",i,node,buffer) );
		}
	}

	if(_parent != 0){
		//generate linking constraints
	double L = _pd->getEffectiveDistribution();

		if(_pd->getReDispatch()){
			_pDeltaPlus = IloNumVarArray(_env,V,0,IloInfinity);
			_pDeltaMinus = IloNumVarArray(_env,V,0,IloInfinity);
			_genReDispatch = IloRangeArray(_env, V, 0, 0);
		}
		if(_pd->getReserve()){		
			_rampingConstraint = IloRangeArray(_env, V, -IloInfinity, 0);
		}
		_effectiveCapacityUp = IloRangeArray(_env,E,-IloInfinity,0);
		_effectiveCapacityDown = IloRangeArray(_env,E,0,IloInfinity);
		_burntOut = IloRangeArray(_env,E,0,IloInfinity);
		_demandRelation = IloRangeArray(_env,V, 0, IloInfinity);
		

		double U,alpha, mx, ecM;
		for(int i=0;i<E;i++){
			srand(subSeed + i);
			U = g->getBranchCapacity(i);
			alpha = rand() / double(RAND_MAX);
			_effectiveCapacity[i]= (L + (1-L)*alpha)*U;
			
			alpha = rand() / double(RAND_MAX);  //uniform 0-1
			if (  alpha > _pd->getProbability() ) _effectiveCapacity[i] = U + 1;
			
			
			if(_pd->getCapacity()){
				mx=_pd->getMaxCapacityPerLine();
				ecM=U + mx - alpha*(U + mx);
				_effectiveCapacityUp[i].setExpr(_parent->getF()[i] - alpha*U - alpha*_pd->getDesignCapacity()[i] - ecM*_x[i]);
				_effectiveCapacityDown[i].setExpr(_parent->getF()[i] + alpha*U + alpha*_pd->getDesignCapacity()[i] + ecM*_x[i]);
			}
			else{
				ecM=U-_effectiveCapacity[i];
				_effectiveCapacityUp[i].setExpr(_parent->getF()[i] - _effectiveCapacity[i] - ecM*_x[i]);
				_effectiveCapacityDown[i].setExpr(_parent->getF()[i] +_effectiveCapacity[i] + ecM*_x[i]);
			}

			_burntOut[i].setExpr(_x[i] - _parent->getX()[i]);
			
			
			//Name Variables and Constraints
			_effectiveCapacityUp[i].setName( getName("ecu",i,node,buffer) );
			_effectiveCapacityUp[i].setName( getName("ecd",i,node,buffer) );
			_burntOut[i].setName( getName("bo",i,node,buffer) );

		}
		for(int i=0;i<V;i++){
			_demandRelation[i].setExpr( _parent->getD()[i] - _d[i]);			
			//Name Variables and Constraints
			_demandRelation[i].setName( getName("C:dr",0,node,buffer) );
			
			if(_pd->getReDispatch()){
				_genReDispatch[i].setExpr(_p[i] - _parent->getP()[i] - _pDeltaPlus[i] + _pDeltaMinus[i]);
				_genReDispatch[i].setName( getName("grd",i,node,buffer) );
				_pDeltaPlus[i].setName( getName("pDeltaPlus",i,node,buffer) );
				_pDeltaMinus[i].setName( getName("pDeltaMinus",i,node,buffer) );
			}		
			if(_pd->getReserve()){
				_rampingConstraint[i].setExpr(_p[i] - _parent->getP()[i] - _parent->getR()[i]);
				_rampingConstraint[i].setName( getName("rc",i,node,buffer) );
			}

		}

		

	}
	
}

void SubProblem::setOutage(int index, bool out){
	if( out ) _x[index].setBounds(1,1);
	else _x[index].setBounds(0,0);
}


double SubProblem::effectiveCapacity(double capacity ){

	double r = rand() / double(RAND_MAX);  //uniform 0-1
	
	double t = _pd->getEffectiveDistribution() + r*(1 - _pd->getEffectiveDistribution());

	return t*capacity;
	//return r;

}

char * SubProblem::getName(const char * prefix,int node, int bus, char * buffer){
	stringstream ss (stringstream::in | stringstream::out);
	
	ss<<prefix;
	ss<<"_";
	ss<<_scenario;
	ss<<",";
	ss<<node;
	ss<<",";
	ss<<bus;
	ss>>buffer;

	return buffer;
}



ostream& operator << (ostream& os, SubProblem& sp){
  os<<sp.getNumber();

	return os;
}
