#include "grid.h"


void Grid::readInFile(const char * fileName){
  cout<<"Trying to read in file "<<fileName<<endl;
	
	int busSize;
	int branchSize;
	
	string temp;
	int tempInt;
	double tempDouble;

	Bus * tempBus;
	Branch * tempBranch;

	ifstream myfile (fileName);

	if(myfile.is_open()){
	  while (myfile.good() ){
			myfile>>temp;
			if(temp == "GRIDDATA"){
				while(temp != "d"){
					myfile>>temp;
				}
				
				//read in grid data
				myfile>>_ID;
				myfile>>busSize;
				myfile>>branchSize;
			}

			if(temp == "BUSDATA"){
				while(temp != "d"){
					myfile>>temp;
				}
				for(int i=0; i<busSize; i++){
					tempBus = new Bus;
					myfile>>tempInt;//tempInt == i???
					myfile>>tempInt;
					tempBus->updateBusNumber(tempInt);
					myfile>>temp;
					tempBus->updateName(temp);
					myfile>>tempDouble;
					if(tempDouble == 0) tempBus->updateNominalDemand(tempDouble);
					else tempBus->updateNominalDemand(-tempDouble);
					myfile>>tempDouble;
					tempBus->updateGenMax(tempDouble);
					tempBus->updateGenMin(0);
					_buses.push_back(*tempBus);
					myfile>>temp;

				}
			}
			if(temp == "BRANCHDATA"){
				while(temp != "d"){
					myfile>>temp;
				}
				for(int i=0; i<branchSize; i++){
					tempBranch = new Branch;
					myfile>>tempInt;//tempInt==i???
					myfile>>tempInt;
					tempBranch->updateSendIndex(tempInt);
					_buses[tempInt].updateConnectedOut(i);
					myfile>>tempInt;
					tempBranch->updateRecIndex(tempInt);
					_buses[tempInt].updateConnectedIn(i);
					myfile>>tempDouble;
					tempBranch->updateCapacity(tempDouble);	//fix  .85->1
					myfile>>tempDouble;
					tempBranch->updateX(tempDouble);
					tempBranch->updateStatus(true);
					_branches.push_back(*tempBranch);
					myfile>>temp;
				}
			}
		}
	}
	else throw FileNotRead();
	
}


ostream& operator << (ostream& os, Grid& g) {
	os<<"Grid:"<<g.getID()<<"-> Buses: "<<g.getBusSize();
	os<<", Branches: "<<g.getBranchSize();
	return os;
}
