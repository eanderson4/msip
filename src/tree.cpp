#include "tree.h"



ostream& operator << (ostream& os, Tree<Node>& t){

	vector<int> nodes;

	os<<"Outcomes: [";
	for(int i=0; i<t._stages; i++){
	  os<<t._outcomes[i]<<" ";
	}
	os<<"] , Stages: "<<t._stages<<endl;
	os<<"Nodes: "<<t._nodes.size();

	int stage=-1;
	for(int i=0; i< (int)t._nodes.size(); i++){
	  if (stage != t._nodes[i]->getStage()){
	    stage = t._nodes[i]->getStage();
	    os<<"\n"<<stage<<": ";
	  }
	  os<<i<<" ";
	}
	os<<"\n";

	/*	os<<"Parents\n";
	stage=-1;
	for(int i=0; i< (int)t._nodes.size(); i++){
	  if (stage != t._nodes[i]->getStage()){
	    stage = t._nodes[i]->getStage();
	    os<<"\n"<<stage<<": ";
	  }
	  os<<t._nodes[i]->getParent()<<" ";
	  }
	os<<"\n";
	*/

return os;
}


ostream& operator << (ostream& os, Tree<NodeData>& t){

	int stage=-1;
	for(int i=0; i< (int)t._nodes.size(); i++){
	  if (stage != t._nodes[i]->getStage()){
	    stage = t._nodes[i]->getStage();
	    os<<"\n"<<stage<<": ";
	  }
	  os<<*t._nodes[i]<<" ";
	}
	os<<"\n";

	return os;
}

ostream& operator << (ostream& os, Tree<SubProblem>& t){

	int stage=-1;
	for(int i=0; i< (int)t._nodes.size(); i++){
	  if (stage != t._nodes[i]->getStage()){
	    stage = t._nodes[i]->getStage();
	    os<<"\n"<<stage<<": ";
	  }
	  os<<*t._nodes[i]<<" ";
	}
	os<<"\n";

	return os;
}
