#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <string>
#include "rootproblem.h"
#include "tree.h"

using namespace std;



int main( int argc, const char* argv[] )
{
  
	if (argc < 2) {
	  std::cerr << "Usage: msip <datafilename>" << std::endl;
	  std::cerr << "Usage: msip <datafilename> <capacity addition file>" <<std::endl;
	  exit(1);
	}
	
	ProblemData *  pd = new ProblemData;
	RootProblem rp;
	RootResult *  solution;

	try
	{
	  pd->readInData(argv[1]);
	  rp.setProblemData(pd);
	  

	  if(argc>=3){
	    cout<<"Read in Capacity file "<<argv[2]<<endl;
	    pd->readInCapAdd(argv[2]);
	  }
	  
	  if(pd->getSIM()){
	    cout<<"SIM: ";
	    if(argc==4) cout<<argv[3]<<endl;
	    rp.simProblemAndOutput(argv[3]);
	    /*
	    solution = rp.simProblem();
	    if(pd->getConsoleOutput()) cout<<*solution<<endl;

	    cout<<"OUTPUT ";
	    
	    //	    cout<<"Final Demand output in "<<pd->getCustomName("final.dem")<<endl;
	    cout<<"Final Demand output in tmp/sim_final.dem"<<endl;
   
	    //	    solution->outputFinalDemand( pd->getCustomName("final.dem") );
	    solution->outputFinalDemand( "tmp/sim_final.dem" );

	    if(pd->getLineOutages()){
	      cout<<"LINES ";
	      //	      solution->outputLineOutages( pd->getCustomName("lines.out") );
	      solution->outputLineOutages( "tmp/sim_lines.out" );
	    }
	    
	    cout<<"DONE\n";
	    */
	  }
	  if(pd->getMSIP()){
	    rp.generateProblem();
	    cout<<rp;
	    if(pd->getMPS()){
	      rp.writeMPS( pd->getOutputName() );
	    }
	    if(pd->getCPLEX()){
	      solution = rp.solveProblem();
	      cout<<*solution<<endl;
	      if(pd->getCapacity()){
		solution->outputDesignCapacity( pd->getCustomName("des.cap") );
	      }
	    }
	  }
	}
	catch(exception& e)
	{
	  cout << e.what() << endl;
	  exit(0);
	}
	

//Line outages in order of most outages to least outages
// 137 132 182 46 138 115 133 122 140 8 112 6 48 106 31 15 45 142 123 37

}
