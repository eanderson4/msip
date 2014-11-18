#include "scenarioproblem.h"


void ScenarioProblem::buildTree(){
	if(_pd != 0){
		_sp.buildTree(_pd->getOutcomes(),_pd->getStages());
	}
	else cout<<"\nProblem Data not Specified\n";
}


ScenarioResult * ScenarioProblem::simProblem(){
	ScenarioResult * sr = new ScenarioResult(_pd);
	
	int seed = _pd->getSeed() + _scenario*100000 + _trial*33000;
	int subSeed;

	if(!_pd->getSubProblemGenerated()){
	  _simSP.generateSubProblem(0,_pd,_scenario);
	}

	//	IloEnv env;
	//	_pd->setEnv(env);

	IloEnv env = _pd->getEnv();
	IloModel mod(env);
	addSimProblem(mod);

 try
  {
    IloCplex cplex(mod);
 
	
	
	cplex.setOut(env.getNullStream());

	Tree<Node> ref;
	ref.buildTree( _pd->getOutcomes(), _pd->getStages());



	Grid* g = _pd->getGrid();
	int E = _pd->getGrid()->getBranchSize();
	int V = _pd->getGrid()->getBusSize();
	
	
	IloNum temp;
	IloNumArray tempA(env);
	IloNumArray tempB(env);
	double R;
	double L = _pd->getEffectiveDistribution();
	double U;
	double f;
	double fabs;
	double alpha;
	double x;
	int parent;
	double tempCount;
	int isDone;

	//fix x variables
	for(int j=0;j<E; j++){
		_simSP.setOutage(j, false);
	}

	//add outages
	for(int j=0; j<_pd->getNumberOutages(_scenario) ; j++){
	  _simSP.setOutage(_pd->getOutages(_scenario,j), true);
	}

	for(int i=0;i<ref.getSize();i++){
		subSeed = seed + i*1000;
		parent= ref[i]->getParent();
		
		if( i!= 0 && _pd->getEnergyStorage() ){//storage logic
			//cout<<"\ni:"<<i<<",pnt:"<<parent<<",es:"<<sr->getETotal(parent);
			for(int j=0; j<V; j++){
				_simSP.setPriorES(j, sr->getE( parent, j ) );
			}
			
		}

		if( i!=0 )	isDone = (int)sr->getDone( parent );
		else		isDone = 0;

		if( isDone ){
			sr->updateDone(i, 1);

			sr->updateDTotal(i,sr->getDTotal(parent));
			sr->updatePDeltaTotal(i,0);
			sr->updateXTotal(i,sr->getXTotal(parent));
		}
		else{
			tempCount=0;
			for(int j=0; j<E; j++){		//set branch outages
			
				if( i!=0) {	
					//OUTAGE LOGIC if not root node
					x = sr->getX(parent,j);
					if( x == 0 ){	//branch not out
						srand(subSeed + j);
						alpha = rand() / double(RAND_MAX);  //uniform 0-1
						U = g->getBranchCapacity(j);
						R = L + (1-L)*alpha;
						R = R*U;
						
					
						alpha = rand() / double(RAND_MAX);  //uniform 0-1
						if (  alpha > _pd->getProbability() ) R = U + 1;
						
						
						f = sr->getF(parent,j); 
						if( f > 0 )  fabs = f;
						else fabs = -f;

						//outage branches
						if( fabs > R) {
							_simSP.setOutage(j,true);
							tempCount += 1;
						}
						else		_simSP.setOutage(j,false);
					}
					else{
						_simSP.setOutage(j,true);
					}
				
				}
			}
			if(tempCount>0)	sr->updateDone(i,0);
			else if(tempCount==0 && i != 0)	sr->updateDone(i,1);
			else sr->updateDone(i,0);

		if(cplex.solve()){
			
//			temp = 	cplex.getObjValue();
			temp = cplex.getValue( _simSP.getSumD() );
			sr->updateDTotal(i,temp);

			cplex.getValues(tempA, _simSP.getD() );
			tempCount=0;
			for(int j=0; j<V; j++){
			  sr->updateD(i, j,tempA[j]);	//bus injects
			}

			cplex.getValues(tempA, _simSP.getP() );
			tempCount=0;
			for(int j=0; j<V; j++){
			  sr->updateP(i, j,tempA[j]);	//bus injects
				if( i!=0 ){
					temp = tempA[j];
					temp -= sr->getP( parent, j);
					sr->updatePDelta(i,j,temp);
					if(temp < 0) tempCount -= 0;//temp;
					else tempCount += temp;
				}
			}
			sr->updatePDeltaTotal(i,tempCount);
			
			cplex.getValues(tempA, _simSP.getF() );
			for(int j=0; j<E; j++){
			  sr->updateF(i,j, tempA[j]);	//branch flows
			}
			cplex.getValues(tempA, _simSP.getX() );
			tempCount=0;
			for(int j=0; j<E; j++){
			  sr->updateX(i,j, tempA[j]);	//branch flows
				tempCount+= tempA[j];
			}
			sr->updateXTotal(i,tempCount);

			if(_pd->getEnergyStorage() ){
				cplex.getValues(tempA, _simSP.getE() );
				for(int j=0; j<V; j++){
				  sr->updateE(i,j, tempA[j]);	//branch flows
				}
				temp = cplex.getValue( IloSum(_simSP.getEDeltaPlus() ) );
				sr->updateEDeltaPlus(i, temp);
				temp = cplex.getValue( IloSum(_simSP.getEDeltaMinus() ) );
				sr->updateEDeltaMinus(i, temp);
				temp = cplex.getValue( IloSum(_simSP.getE() ) );
				sr->updateETotal(i, temp);
			}
		}
		}
	}
	//	mod.end();
	//	env.end();

 }
  catch (IloException e)
  {
    cout << "An exception occurred. Exception Nr. " << e << endl;
  }

	return sr;
}

void ScenarioProblem::addSimProblem(IloModel mod){

	mod.add(_simSP.getP());
	mod.add(_simSP.getD());
	mod.add(_simSP.getTheta());
	
	if(_pd->getGeneratorSwitch()){
		mod.add(_simSP.getY());
	}
	
	if(_pd->getEnergyStorage()){
		mod.add(_simSP.getE());
		mod.add(_simSP.getEDeltaPlus());
		mod.add(_simSP.getEDeltaMinus());
		mod.add(_simSP.getEnergyStorage());
	}

//	if(_pd->getReDispatch()&&index>0){
//		mod.add(_simSP.getPDelta());
//		mod.add(_simSP.getGenReDispatch());
//	}
	if(_pd->getReserve()){
		mod.add(_simSP.getR());
		mod.add(_simSP.getReserveMargin());
	//	if(index>0)	mod.add(_simSP.getRampingCapacity());
	}
	
	mod.add(_simSP.getF());
	mod.add(_simSP.getX());

	if(_pd->getLineSwitch()){
		mod.add(_simSP.getZ());
		mod.add(_simSP.getW());
	}
	
	mod.add(_simSP.getEnergyConservation());
	mod.add(_simSP.getPhaseAngleUp());
	mod.add(_simSP.getPhaseAngleDown());
	mod.add(_simSP.getLineLimitsUp());
	mod.add(_simSP.getLineLimitsDown());

	if(_pd->getCapacity()){
		mod.add(_simSP.getLineLimitsUpM());
		mod.add(_simSP.getLineLimitsDownM());
	}

	if(_pd->getLineSwitch()){
		mod.add(_simSP.getLineSwitch());
		mod.add(_simSP.getLineBurnt());
	}
	mod.add(_simSP.getGenLimitsUp());
	mod.add(_simSP.getGenLimitsDown());

	if(_pd->getEnergyStorage()){
		mod.add(	IloMaximize(	_pd->getEnv(),	_simSP.getSumD()	
					-.1*(  IloSum(_simSP.getEDeltaPlus()) + IloSum(_simSP.getEDeltaMinus())	)	
					));
	}
	else{
		mod.add(	IloMaximize(	_pd->getEnv(),	_simSP.getSumD() ));	
	}


}

void ScenarioProblem::generateProblem(){

	for(int i=0;i<_sp.getSize();i++){
		if(i>0){
			_sp[i]->generateSubProblem(i,_pd,_scenario,_sp[_sp.getParent(i)]);
		}
		else{
			_sp[i]->generateSubProblem(i,_pd, _scenario);
			
			for(int j=0; j<_pd->getNumberOutages(_scenario) ; j++){
			    _sp[i]->setOutage(_pd->getOutages(_scenario,j), true);
			}
		}

		try{  
  			addSubProblem(i);
		}
		catch (IloEmptyHandleException e)
		{
			cout << "An exception occurred. Exception " << e << endl;
		}

	}


	int stage;
	_sumWeightedDemand=IloExpr(_pd->getEnv());	
	for(int i=0;i<_sp.getSize();i++){
		stage=_sp[i]->getStage();
		_sumWeightedDemand += _pd->getObjWeights(stage)*_sp[i]->getSumD();

		if(i!=0 && _pd->getReDispatch() && _pd->getPenaltyDispatch() ){
			_sumWeightedDemand -= _pd->getPenaltyWeight()*IloSum( _sp[i]->getPDeltaPlus() );


		}
		
	}	
	


}


void ScenarioProblem::addSubProblem(int index){
	_pd->getModel().add(_sp[index]->getP());
	_pd->getModel().add(_sp[index]->getD());
	_pd->getModel().add(_sp[index]->getTheta());
	
	if(_pd->getGeneratorSwitch()){
		_pd->getModel().add(_sp[index]->getY());
	}
	
	if(_pd->getReDispatch()&&index>0){
		_pd->getModel().add(_sp[index]->getPDeltaPlus());
		_pd->getModel().add(_sp[index]->getPDeltaMinus());
		_pd->getModel().add(_sp[index]->getGenReDispatch());
	}
	if(_pd->getReserve()){
		_pd->getModel().add(_sp[index]->getR());
		_pd->getModel().add(_sp[index]->getReserveMargin());
		if(index>0)	_pd->getModel().add(_sp[index]->getRampingCapacity());
	}
	
	_pd->getModel().add(_sp[index]->getF());
	_pd->getModel().add(_sp[index]->getX());

	if(_pd->getLineSwitch()){
		_pd->getModel().add(_sp[index]->getZ());
		_pd->getModel().add(_sp[index]->getW());
	}
	
	_pd->getModel().add(_sp[index]->getEnergyConservation());
	_pd->getModel().add(_sp[index]->getPhaseAngleUp());
	_pd->getModel().add(_sp[index]->getPhaseAngleDown());
	_pd->getModel().add(_sp[index]->getLineLimitsUp());
	_pd->getModel().add(_sp[index]->getLineLimitsDown());

	if(_pd->getCapacity()&&_pd->getMaxLineChanges()>0){
		_pd->getModel().add(_sp[index]->getLineLimitsUpM());
		_pd->getModel().add(_sp[index]->getLineLimitsDownM());
	}

	if(_pd->getLineSwitch()){
		_pd->getModel().add(_sp[index]->getLineSwitch());
		_pd->getModel().add(_sp[index]->getLineBurnt());
	}
	_pd->getModel().add(_sp[index]->getGenLimitsUp());
	_pd->getModel().add(_sp[index]->getGenLimitsDown());

	if(index>0){
		_pd->getModel().add(_sp[index]->getEffectiveCapacityUp());
		_pd->getModel().add(_sp[index]->getEffectiveCapacityDown());
		_pd->getModel().add(_sp[index]->getBurntOut());
		_pd->getModel().add(_sp[index]->getDemandRelation());
	}
}



ostream& operator << (ostream& os, ScenarioProblem& mp){
	os<<mp._sp<<endl;
	return os;
}
