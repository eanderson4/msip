#include "node.h"


Node::Node(int n, int stage, int parent){ 
	_number = n; 
	_stage=stage; 
	_parent=parent; 
}



ostream& operator << (ostream& os, Node& n){
	os<<"#"<<n._number<<" <- "<<n._parent<<" ("<<n._stage<<" ) \n";

	return os;
}
