#ifndef BRANCH_H
#define BRANCH_H


#include <iostream>
using namespace std;


class Branch {

	friend ostream& operator << (ostream& os, Branch& b);

public:
	//constructors and destructors
	Branch(){}
	~Branch(){}

	int getSendIndex(){ return _sendIndex; }
	int getRecIndex(){ return _recIndex; }
	double getCapacity(){ return _U; }
	double getX(){ return _X; }
	bool getStatus(){ return _status; }

	void updateSendIndex(int sendIndex){ _sendIndex = sendIndex; }
	void updateRecIndex(int recIndex){ _recIndex = recIndex; }
	void updateStatus(bool status){ _status = status; }
	void updateCapacity(double U){ _U = U; }
	void updateX(double X){ _X = X; }

private:
	int _sendIndex, _recIndex;
	double _U;
	double _X;
	bool _status;


};



#endif
