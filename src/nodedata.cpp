#include "nodedata.h"

ostream& operator << (ostream& os, NodeData& n){
	os<<*n._data;
	return os;
}
