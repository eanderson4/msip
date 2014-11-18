#include "rootproblem.h"


RootResult * RootProblem::simProblem(){
	
	RootResult* r =  new RootResult(_pd,true);
	
	if(checkParameters()){


		ScenarioProblem * blank;
		ScenarioResult * sr;

		Tree<Node> ref(_pd->getOutcomes(),_pd->getStages());
		cout<<"\nRefrence Tree\n";
		cout<<ref;

		for(int i=0; i<_pd->getScenarios();i++){
		  for (int j=0; j<_pd->getTrials();j++){
		    cout<<":"<<i<<", "<<j<<endl;
			blank = new ScenarioProblem;
			blank->setProblemData(_pd);
			blank->setScenario(i);
			blank->setTrial(j);
			//			blank->buildTree();
			sr = blank->simProblem();
			sr->setScenario(i+1);
			r->updateSR(sr);
			delete blank;
			//			_sc.push_back(blank);
		  }
		}
	}

	return r;

}

void RootProblem::simProblemAndOutput(const char * name){
	
  ScenarioProblem * blank;
  ScenarioResult * sr;
  
  Tree<Node> ref(_pd->getOutcomes(),_pd->getStages());
  cout<<"\nRefrence Tree\n";
  cout<<ref;
  int stages = ref.getStages();
  int * start = ref.getStart();

  string ndem("./");
  ndem += name;
  ndem += ".dem";
  string nlin("./");
  nlin += name;
  nlin += ".lin";
  string nform("./");
  nform += name;
  nform += ".form";
  string ncas("./");
  ncas += name;
  ncas += ".cas";
  string npwr("./");
  npwr += name;
  npwr += ".pwr";
  string nlf("./");
  nlf += name;
  nlf += ".lf";


  ofstream mydem( ndem.c_str() );
  ofstream mylin( nlin.c_str() );
  ofstream myform( nform.c_str() );
  ofstream mycas( ncas.c_str() );
  ofstream mypwr( npwr.c_str() );
  ofstream mylf( nlf.c_str() );
  cout<<"Demand output to "<<ndem.c_str()<<endl;
  cout<<"Line outages to "<<nlin.c_str()<<endl;
  cout<<"Formatted output to "<<nform.c_str()<<endl;
  cout<<"Cascade output to "<<ncas.c_str()<<endl;
  cout<<"Power Injects to "<<npwr.c_str()<<endl;
  cout<<"Line Flows to "<<nlf.c_str()<<endl;

  if( mydem.is_open() && mylin.is_open() ){
  
    for(int i=0; i<_pd->getScenarios();i++){
      for (int j=0; j<_pd->getTrials();j++){
	cout<<":"<<i<<", "<<j<<endl;
	if(i==0&&j==0){
	  blank = new ScenarioProblem;
	}
	blank->setProblemData(_pd);
	blank->setScenario(i);
	blank->setTrial(j);
	//	blank->buildTree();
	sr = blank->simProblem();
	sr->setScenario(i);
	
	//output current results
	for(int j=start[stages]; j<start[stages+1]; j++){
	  mydem<<sr->getDTotal(j)<<"  ";
	  for(int k=0; k<(int)_pd->getGrid()->getBranchSize(); k++){
	    int x = sr->getX(j,k);
	    if(x==1) mylin<<k<<" ";
	  }
	}
	for(int j=start[stages]; j<start[stages+1]; j++){
	  
	  int parent=ref.getParent(j);
	  int back[stages+2];
	  
	  back[0]=j;
	  back[1]=parent;
	  for(int c=1;c<stages-1;c++){ 
	    parent = ref.getParent(parent);   
	    back[c+1]=parent;
	  }
	  back[stages]=ref.getParent(parent);

	  int b=back[stages];
	  mycas<<sr->getDTotal(b)<<" ";
	  for(int k=0; k<(int)_pd->getGrid()->getBranchSize(); k++){
	    int xi = sr->getX(b,k);
	    if (xi==1) mycas<<k<<" ";
	    mylf<<sr->getF(b,k)<<" ";
	  }
	  mycas<<"- ";
	  mylf<<"- ";
	  for(int n=0; n<(int)_pd->getGrid()->getBusSize(); n++){
	    double p = sr->getP(b,n);
	    p = p -sr->getD(b,n);
	    mypwr<<p<<" ";
	  }
	  mypwr<<"- ";
	  int a;
	  for(int c=stages-1;c>=0;c--){
	    a = back[c];	  
	    mycas<<sr->getDTotal(a)<<" ";
	    for(int k=0; k<(int)_pd->getGrid()->getBranchSize(); k++){
	      int xi=sr->getX(b,k);
	      int x = sr->getX(a,k);
	      if(x==1&&xi!=1) mycas<<k<<" ";
	      mylf<<sr->getF(a,k)<<" ";

	    }
	    if (c!=0) mycas<<"- ";
	    if (c!=0) mylf<<"- ";

	    for(int n=0; n<(int)_pd->getGrid()->getBusSize(); n++){
	      double p = sr->getP(a,n);
	      p = p -sr->getD(a,n);
	      mypwr<<p<<" ";
	    }
	    mypwr<<"- ";

	    b = a;
	  }
       	  
	  mycas<<"\n";
	  mylf<<"\n";
	  mypwr<<"\n";

	  int parXi = ref.getParent(parent);
	  for(int k=0; k<(int)_pd->getGrid()->getBranchSize(); k++){
	    int xi = sr->getX(parXi,k);
	    if (xi==1) myform<<k<<" ";
	  }
	  myform<<": ";
	  for(int k=0; k<(int)_pd->getGrid()->getBranchSize(); k++){
	    int x =sr->getX(parent,k);
	    int xi=sr->getX(parXi,k);
	    if(x==1&&xi!=1) myform<<k<<" ";
	  }
	  myform<<"- "<<sr->getDTotal(j)<<"\n";  //<<" - "<<sr->getDTotal(parent)<<"\n";

	}
	
	//	delete blank;
	delete sr;
	
	mydem.flush();
	mylin.flush();
	myform.flush();
	mycas.flush();
	mypwr.flush();
	mylf.flush();
      }
    }

    mydem.close();
    mylin.close();
    myform.close();
    mycas.close();
    mypwr.close();
    mylf.close();


  }

}




void RootProblem::generateProblem(){

	if(checkParameters()){
		IloExpr obj(_pd->getEnv(),0);

		ScenarioProblem * blank;
		
		cout<<"Building Problem\n";
		
		for(int i=0; i<_pd->getScenarios();i++){
		  blank = new ScenarioProblem;
		  blank->setProblemData(_pd);
		  blank->setScenario(i);
		  blank->buildTree();
		  blank->generateProblem();
		  obj += blank->getWeightedDemand();
		  _sc.push_back(blank);
		}
	
		_pd->setObjective(IloMaximize(_pd->getEnv(),obj));
		
	}

}


void RootProblem::writeMPS( const char* name){
  IloCplex cplex(_pd->getModel());
  cplex.exportModel( name );
  cout<<"MPS model wrote to "<<name<<endl;
}

RootResult * RootProblem::solveProblem(){

	RootResult* r =  new RootResult(_pd);
	IloCplex cplex(_pd->getModel());

	if(_pd->getCPLEX()){
	  if(_pd->getTimeLimit()!=0)  cplex.setParam(IloCplex::TiLim, _pd->getTimeLimit());
	  if(_pd->getSolLimit()!=0)  cplex.setParam(IloCplex::IntSolLim, _pd->getSolLimit());

	  cplex.setOut(*_pd->getStream());
	
	  double stopGap=_pd->getGapLimit();
	  cplex.setParam( IloCplex::MIPEmphasis, 1);
	  cplex.setParam( IloCplex::EpGap, stopGap);
	  cplex.setParam( IloCplex::HeurFreq, 5);

	  cout<<"\n Stop Criteria: \n";
	  cout<<"Gap <= "<<stopGap<<"\n";
	  if(_pd->getTimeLimit()!=0) cout<<"Time >= "<<_pd->getTimeLimit()<<"\n";
	  else cout<<"No Time Limit\n";
	  if(_pd->getSolLimit()!=0) cout<<"Feasible Solutions >= "<<_pd->getSolLimit()<<"\n\n";
	  else cout<<"No Solution Limit\n\n";

	  if(cplex.solve()){

		cout<<"SOLVED\n";
		IloNum mipGap;
		mipGap = cplex.getMIPRelativeGap();
		r->updateGap((double)mipGap);

		IloNum temp;

		if(_pd->getCapacity()){
			temp = cplex.getValue( IloSum(_pd->getDesignCapacity()));
			r->updateDesignCapacity(temp);
		}

		cout<<"StartDataCollection\n";
		for(int i=0; i<(int)_sc.size();i++){
			for(int j=0; j<_sc[i]->getSize(); j++){
				temp = 	cplex.getValue(_sc[i]->getSubProblem(j)->getSumD());
				r->updateDTotal(i,j,temp);
			
				temp = cplex.getValue(_sc[i]->getSubProblem(j)->getSumX());
				r->updateXTotal(i,j,temp);

				for(int k=0;k<_pd->getGrid()->getBranchSize(); k++){
					temp=cplex.getValue(_sc[i]->getSubProblem(j)->getX()[k]);
					r->updateX(i,j,k,temp);
				}
				if(_pd->getCapacity() ){
					for(int k=0;k<_pd->getGrid()->getBranchSize();k++){
						temp=cplex.getValue(_pd->getDesignCapacity()[k]);
						r->addedCapacity(temp);
					}
				}

				if(_pd->getLineSwitch()){
					temp = cplex.getValue(_sc[i]->getSubProblem(j)->getSumW());
					r->updateWTotal(i,j,temp);		
			
					temp = cplex.getValue(_sc[i]->getSubProblem(j)->getSumZ());
					r->updateZTotal(i,j,temp);
				}	
				if(_pd->getReDispatch() && (j > 0) ){
					temp = cplex.getValue(_sc[i]->getSubProblem(j)->getSumPDeltaPlus());
					if(temp<0) temp=-temp;
					if(temp<.0000001) temp=0;
			
					r->updatePDeltaTotal(i,j,temp);		
				}
				if(_pd->getReserve() ){
					temp = cplex.getValue(_sc[i]->getSubProblem(j)->getSumR());
					if(temp<0) temp=-temp;
					if(temp<.0000001) temp=0;

					temp = temp/( temp + cplex.getValue(_sc[i]->getSubProblem(j)->getSumP()));
					
					r->updateRTotal(i,j,temp);		
				}

			}
		}
		cout<<"\nData Collection DoNE\n";
	  }
	  else{
		cout<<"Solution Not Found\n";
	  }
	}


	cplex.end();


	return r;
}



ostream& operator << (ostream& os, RootProblem& rp){
	os<<"\n";
	os<<*rp._pd;
	os<<"\n";

	for(int i=0;i<(int)rp._sc.size();i++){
		os<<"Scenario Cascade Tree: "<<i+1;
		os<<*rp._sc[i]<<endl;
	}
	return os;
}
