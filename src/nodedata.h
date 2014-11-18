#ifndef NODEDATA_H
#define NODEDATA_H

#include <vector>
#include "node.h"

#define DATA		1

using namespace std;

class NodeData : public Node{

	friend ostream& operator <<(ostream& os, NodeData& n);

public:

        NodeData(int n,int stage, int parent):Node(n,stage,parent){_data= new double;}
	~NodeData(){ delete _data;}

	void setData(double d){ *_data=d; }
	double getData(){ return *_data; }

	void initV(int maxData){ 
	  _maxData=maxData; _dataV.resize(_maxData,0); }
	void setDataV(int n, double d){ _dataV[n]=d; }
	double getDataV(int j){ return _dataV.at(j); }

private:

	double * _data;
	vector<double> _dataV;

	int _maxData;
	
	static const int _type = DATA;
};

#endif
