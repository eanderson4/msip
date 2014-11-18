#ifndef NODE_H
#define NODE_H

#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

class Node {

	friend ostream& operator << (ostream& os, Node& n);

public:
	Node(){}
	Node(int, int, int);
	~Node(){}

	int getNumber(){return _number;}
	int getStage(){return _stage;}
	int getParent(){return _parent;}

private:
	int _number;
	int _stage;
	int _parent;


	static const int _type = 0;

};

#endif
