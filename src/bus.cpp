#include "bus.h"


ostream& operator << (ostream& os, Bus& b) {
	os<<"#"<<b._busNumber<<" Gen ["<<b._genMin<<","<<b._genMax<<"] Dem "<<b._nominalDemand;
	return os;
}
