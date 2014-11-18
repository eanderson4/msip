#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <fstream>
#include <vector>
#include "node.h"
#include "nodedata.h"
#include "subproblem.h"



using namespace std;


	template <class NodeType>
class Tree {

	friend ostream& operator << (ostream& os, Tree<Node>& t);
	friend ostream& operator << (ostream& os, Tree<NodeData>& t);
	friend ostream& operator << (ostream& os, Tree<SubProblem>& t);

public:

	Tree(){}
	Tree(int * outcomes, int stages){  buildTree(outcomes,stages); }
	~Tree(){
		for(int i=0; i<(int)_nodes.size(); i++){
			delete _nodes[i];
		}
		//		delete _num;
		//delete _start;
	}
                                              
	
	void buildTree(int * outcomes, int stages){
		_outcomes=outcomes;
		_stages=stages;
		_start = new int[stages+2];
		_num = new int[stages+2];
		_start[0] = 0;
		_num[0] = 1;
		
		for(int i =1; i<=stages; i++){
		  _num[i] = _num[i-1]*outcomes[i-1];
		  _start[i] = _start[i-1] + _num[i-1];
		}
		_start[stages+1] = _start[stages] + _num[stages];
		_totalNodes = _start[stages+1];


		int n=0;
		NodeType * temp = new NodeType(n,0,0);
		_nodes.push_back(temp);
		n++;

		for(int i=1; i<=stages; i++){
		  int pos = 0;
		  for(int j=_start[i]; j<_start[i+1]; j++){
		    int testparent = (int) pos/outcomes[i-1];
		    if (i>0)    testparent = _start[i-1] + testparent;
		    else testparent = 0;

		    temp = new NodeType(n,i,testparent);
		    _nodes.push_back(temp);
		    n++;
		    pos++;
		  }
		}

	}

	int getSize(){ return _totalNodes; }
	int * getOutcomes(){ return _outcomes; }
	int * getStart(){ return _start; }
	int getStages(){ return _stages; }

	int getNumber(int pos){ return _nodes[pos]->getNumber(); }
	int getParent(int pos){ return _nodes[pos]->getParent(); }

	NodeType * getNode(int n){ return _nodes[n]; }
	NodeType * operator[](const int index){ return _nodes[index]; }

	void initV(int maxData){
	  for( int i =0; i<(int)_nodes.size(); i++){
	    _nodes[i]->initV(maxData);
	  }
	}



private:

int * _outcomes;
int * _start;
int * _num;
int _stages;
int _totalNodes;

vector<NodeType *> _nodes;

};


#endif
