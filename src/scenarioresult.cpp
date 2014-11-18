#include "scenarioresult.h"


ScenarioResult::ScenarioResult(ProblemData* p){
	_pd = p;
	
	int * outcomes = p->getOutcomes();
	int stages = p->getStages();

	int bus = p->getGrid()->getBusSize();
	int branch = p->getGrid()->getBranchSize();

	_dTotal.buildTree(outcomes, stages);
	_x.buildTree(outcomes, stages);
	_x.initV(branch);
	_xTotal.buildTree(outcomes, stages);
	if(_pd->getSimProblem()){
		_d.buildTree(outcomes, stages);
		_d.initV(bus);
		_p.buildTree(outcomes, stages);
		_p.initV(bus);
		_pDelta.buildTree(outcomes, stages);
		_pDelta.initV(bus);
		_f.buildTree(outcomes, stages);	
		_f.initV(branch);
		_e.buildTree(outcomes, stages);
		_e.initV(bus);
		_done.buildTree(outcomes, stages);
	}
	if(_pd->getLineSwitch()){
		_wTotal.buildTree(outcomes, stages);
		_zTotal.buildTree(outcomes, stages);
	}
	if(_pd->getReserve()){
		_rTotal.buildTree(outcomes, stages);
	}
	if(_pd->getReDispatch()){
		_pDeltaTotal.buildTree(outcomes, stages);
	}
	if(_pd->getEnergyStorage()){
		_eDeltaPlus.buildTree(outcomes, stages);
		_eDeltaMinus.buildTree(outcomes, stages);
		_eTotal.buildTree(outcomes, stages);
	}

}


ostream& operator << (ostream& os, ScenarioResult& r){

	os<<"\n\nD_total";
	os<<r._dTotal;
	//LINE OUTAGES
/*	for(int i=0; i< r._x.getSize(); i++){
		for(int j=0; j< r._pd->getGrid()->getBranchSize(); j++){
			if( r.getX(i,j) == 1 ) os<<" "<<j<<" ";
		}
	}*/
	os<<"\n\nx_total";
	os<<r._xTotal;
/*	if(r._pd->getLineSwitch()){
		os<<"\n\nw_total";
		os<<r._wTotal;
		os<<"\n\nz_total";
		os<<r._zTotal;
	}
	if(r._pd->getReDispatch()){
		os<<"\n\npDelta_total";
		os<<r._pDeltaTotal;
	}
	if(r._pd->getReserve()){
		os<<"\n\nr_total";
		os<<r._rTotal;
	}
	if(r._pd->getSimProblem() ){
		os<<"\n\ndone";
		os<<r._done;
	}
	if(r._pd->getEnergyStorage() ){
		os<<"\n\nePlus";
		os<<r._eDeltaPlus;
		os<<"\n\neMinus";
		os<<r._eDeltaMinus;
		os<<"\n\neTotal";
		os<<r._eTotal;
	}
*/
	return os;	
}
