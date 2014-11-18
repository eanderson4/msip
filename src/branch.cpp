#include "branch.h"



ostream& operator << (ostream& os, Branch& br) {
	if(br.getStatus()) os<<"On";
	else os<<"Off";
	os<<": ["<<br.getSendIndex()<<", "<<br.getRecIndex()<<"], Limit: "<<br.getCapacity()<<", X: "<<br.getX();
	return os;
}

