#include "rootresult.h"

RootResult::RootResult(ProblemData* p, bool sim){
	_pd=p;

	if( !sim){
		ScenarioResult * blank;

		for(int i=0; i<_pd->getScenarios(); i++){
			blank = new ScenarioResult(_pd);
			blank->setScenario(i);
			_sr.push_back(blank);
		}
	}
}

void RootResult::outputDesignCapacity(const char* file){
  ofstream myfile( file );
  if( _pd->getCapacity()){
    myfile<<"Design Capacity -all \n";
    for(int i=0; i<_pd->getGrid()->getBranchSize(); i++){
      myfile<<" "<<_addedCapacity[i]<<" ";
    }
    myfile<<"\n";
  }
  myfile.close();

}

void RootResult::outputFinalDemand(const char* file){
  //  ofstream myfile( file );
  ofstream myfile( "tmp/sim_final.dem" );
  if (myfile.is_open()){
      myfile<<"Final Demand"<<endl;
      
      Tree<Node> ref;
      ref.buildTree(_pd->getOutcomes(),_pd->getStages());
      int stages = ref.getStages();
      int * start = ref.getStart();
      
      
      for(int ian=0; ian<(int)_sr.size(); ian++){
	for(int jeff=start[stages]; jeff<start[stages+1]; jeff++){
	       myfile<<_sr[ian]->getDTotal(jeff)<<"  ";
	}
      }
      
      myfile.close();
    }
}

void RootResult::outputLineOutages(const char* file){
  //  ofstream myfile( file );
  ofstream myfile( "tmp/sim_lines.out" );
  if(myfile.is_open()){
    myfile<<"Line Outages"<<endl;
  
    Tree<Node> ref;
    ref.buildTree(_pd->getOutcomes(),_pd->getStages());
    int stages = ref.getStages();
    int * start = ref.getStart();
    
  
    for(int i=0; i<(int)_sr.size(); i++){
      for(int j=start[stages]; j<start[stages+1]; j++){
	for(int k=0; k<(int)_pd->getGrid()->getBranchSize(); k++){
	  int x = _sr[i]->getX(j,k);
	  if(x==1) myfile<<k<<" ";
	}
      }
    }
    
    myfile.close();
  }
}

ostream& operator << (ostream& os, RootResult& r){
	cout<<"\nOutput Results\n";
	if( !r._pd->getSimProblem()){
	  os<<"\n\nMIP Gap: "<<r._mipGap;
	  if( r._pd->getCapacity()){
		os<<"\n\nDesign Capacity -all \n";
		for(int i=0; i<r._pd->getGrid()->getBranchSize(); i++){
		  os<<" "<<r._addedCapacity[i]<<" ";
		}
		os<<"\n";
	  }
	}
	
	for(int i=0; i<(int)r._sr.size();i++){
	    os<<"\n\n\n ========= Scenario "<<r._sr[i]->getScenario()<<" ==========   ";
		os<<*r._sr[i];
	}
	return os;	
}
