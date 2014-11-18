#include "problemdata.h"


bool ProblemData::checkParameters(){

	return true;
}

const char * ProblemData::getCustomName(const char* affix){
  stringstream ss;
  if( !_name.empty()){
    ss << "tmp/"<<_name.c_str() ;
  }
  else{
    ss << "tmp/data" ;
  }
  ss << "_" << affix; 
  string temp;
  ss>>temp;

  return temp.c_str();
}


void ProblemData::readInCapAdd( const char* file){
  ifstream myfile (file);
  string line;
  string dataText;
  double dataDouble;  

  string name(file);
  istringstream lines( name );
  getline( lines, name, '/');
  getline( lines, name, '.');

  

  if(myfile.is_open()){
    cout<<file<<" opened, read capacity additions"<<endl;
    getline(myfile,line);
    stringstream str(line);
    string text;
    getline(str,text,'-');
    str >> dataText;
    if(dataText=="all"){
      getline(myfile,line);
      stringstream data(line);
      data >> dataDouble;
      for(int i=0; data.good(); i++){
        _g->setBranchCapacity(i, _g->getBranchCapacity(i) + dataDouble);
	data >> dataDouble;
      }
    }
    if(dataText=="individual"){
      while(getline(myfile,line)){
	stringstream stream(line);
	double index;
	double addition;
	stream >> index;
	stream >>addition;
	_g->setBranchCapacity(index, _g->getBranchCapacity(index) + addition); 
	cout<<index<<": "<<_g->getBranchCapacity(index)<<" + "<<addition<<", ";
      }
      cout<<" DONE"<<endl;
    }
    
   
    //Rename problem to include design
    stringstream ss(file);
    string des_name;
    getline(ss,des_name,'.');
    stringstream new_name;
    new_name << des_name << "_" <<_name;
    new_name >> _name;

    _name = name;

  }
  else{
    cout<<"Could not open "<<file<<", no capacity additions made"<<endl;
  }
  

}


void ProblemData::readInData( const char* file){
  ifstream myfile (file);
  string line;
  string dataText;
  double dataDouble;
  int dataInt;
  bool dataBool;
  vector<int> * dataVectorInt;

  if(myfile.is_open()){
    cout<<"\n"<<file<<" opened, reading problem data\n"<<endl;
    while(getline(myfile,line)){
      stringstream str(line);
      string text;

      getline(str,text,'=');
      
      if(text=="name"){
	str >> dataText;
	cout<<text<<": "<<dataText<<endl;
	_name=dataText;
      }
      else if(text=="consoleoutput"){
	str >> dataText;
	cout<<text<<": "<<dataText<<endl;
	if(dataText=="no"){
	  _consoleOutput=false;
	}
      }
      else if(text=="lineoutage"){
	str >> dataText;
	cout<<text<<": "<<dataText<<endl;
	if(dataText=="yes"){
	  _outputLineOutages=true;
	}
      }
      else if(text=="solvemethod"){
	str >> dataText;
	cout<<text<<": "<<dataText<<endl;
	if(dataText=="mps"){
	  _solveMPS=true;
	  _solveCPLEX=false;
	}
	if(dataText=="cplex"){
	  _solveMPS=false;
	  _solveCPLEX=true;
	}
       }
       else if(text=="problemtype"){
	 str >> dataText;
	 cout<<text<<": "<<dataText<<endl;
	 if(dataText=="sim"){
	   _simProblem=true;
	   _msipProblem=false;
	 }
	 if(dataText=="msip"){
	   _simProblem=false;
	   _msipProblem=true;
	 }
       }
       else if(text=="gridfile"){
	 str >> dataText;
	 cout<<text<<": "<<dataText<<endl;
	 _g = new Grid;
	 _g->readInFile(dataText.c_str());
	 cout<<*_g<<endl;
       }
       else if(text=="p"){
	 str >> dataDouble;
	 cout<<text<<": "<<dataDouble<<endl;
	 _probability=dataDouble;
       }
       else if(text=="L"){
	 str >> dataDouble;
	 cout<<text<<": "<<dataDouble<<endl;
	 _effectiveDistributionL=dataDouble;
       }
       else if(text=="outcomes"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt<<endl;
	 _outcomes= new int[_stages];
	 fill_n(_outcomes,_stages,dataInt);
       }
       else if(text=="outcomesV"){
	 _outcomes = new int[_stages];
	 int n=0;
	 while (str.good()){
	   str >>dataInt;
	   if(str.good())   _outcomes[n]=dataInt;
	   n++;

	 }

	 cout<<text<<": ";
	 for(int i=0; i<_stages; i++){
	   cout<<_outcomes[i]<<" ";
	 }
	 cout<<"\n";
       }
       else if(text=="stages"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt<<endl;
	 _stages=dataInt;
       }
       else if(text=="trials"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt<<endl;
	 _trials=dataInt;
       }
       else if(text=="scenarios"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt<<endl;
	 _scenarios=dataInt;
	 if(dataInt>4){
	   cerr << " Too many scenarios.  Only 4 scenarios will be read in. "<<endl;
	   _scenarios=4;
	 }
       }
       else if(text=="a"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt;
	 dataVectorInt = new vector<int>;
	 dataVectorInt->push_back(dataInt);
	 for(int i=1;str.good();i++){
	   str >> dataInt;
	   cout << " " << dataInt;
	   dataVectorInt->push_back(dataInt);
	 }
	 cout<<endl;
	 _outages.push_back(*dataVectorInt);
	 delete dataVectorInt;
       }
       else if(text=="b"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt;
	 dataVectorInt = new vector<int>;
	 dataVectorInt->push_back(dataInt);
	 while(str.good()){
	   str >> dataInt;
	   cout << " " << dataInt;
	   dataVectorInt->push_back(dataInt);
	 }
	 cout<<endl;
	 _outages.push_back(*dataVectorInt);
	 delete dataVectorInt;
       }
       else if(text=="c"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt;
	 dataVectorInt = new vector<int>;
	 dataVectorInt->push_back(dataInt);
	 while(str.good()){
	   str >> dataInt;
	   cout << " " << dataInt;
	   dataVectorInt->push_back(dataInt);
	 }
	 cout<<endl;
	 _outages.push_back(*dataVectorInt);
	 delete dataVectorInt;
       }
       else if(text=="d"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt;
	 dataVectorInt = new vector<int>;
	 dataVectorInt->push_back(dataInt);
	 while(str.good()){
	   str >> dataInt;
	   cout << " " << dataInt;
	   dataVectorInt->push_back(dataInt);
	 }
	 cout<<endl;
	 _outages.push_back(*dataVectorInt);
	 delete dataVectorInt;
       }
       else if(text=="objweight"){
	 str >> dataDouble;
	 cout<<text<<": "<<dataDouble;
	 _objWeights.push_back(dataDouble);
	 while(str.good()){
	   str >> dataDouble;
	   cout << " " << dataDouble;
	 _objWeights.push_back(dataDouble);
	 }
	 cout<<endl;
       }
       else if(text=="descapacity"){
	 str >> dataBool;
	 cout<<text<<": "<<dataBool<<endl;
	 _allowCapacity=dataBool;
       }
       else if(text=="minperline"){
	 str >> dataDouble;
	 cout<<text<<": "<<dataDouble<<endl;
	 _minCapacityPerLine = dataDouble;
       }
       else if(text=="maxperline"){
	 str >> dataDouble;
	 cout<<text<<": "<<dataDouble<<endl;
	 _maxCapacityPerLine = dataDouble;
       }
       else if(text=="totalbudget"){
	 str >> dataDouble;
	 cout<<text<<": "<<dataDouble<<endl;
	 _maxCapacity = dataDouble;
       }
       else if(text=="maxlines"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt<<endl;
	 _maxLineChanges = dataInt;
       }
       else if(text=="seed"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt<<endl;
	 _seed = dataInt;
       }
       else if(text=="outputfile"){
	 str >> dataText;
	 cout<<text<<": "<<dataText<<endl;
	 _outputFile = dataText;
       }
       else if(text=="timelimit"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt<<endl;
	 _timeLimit=dataInt;
       }
       else if(text=="solutionlimit"){
	 str >> dataInt;
	 cout<<text<<": "<<dataInt<<endl;
	 _solLimit=dataInt;
       }
       else if(text=="gaplimit"){
	 str >> dataDouble;
	 cout<<text<<": "<<dataDouble<<endl;
	 _gapLimit=dataDouble;
       }
       else{
	 cout<<text<<endl;
       }



    
    }
    if(_allowCapacity){
      designCapacity(_minCapacityPerLine,_maxCapacityPerLine,_maxCapacity,_maxLineChanges);
    }
    cout<<"\nFile read in complete"<<endl;
  }
  else{
    cerr << "Unable to open file " << file << endl;
  }



}


char * ProblemData::getName(const char * prefix, int bus, char * buffer){
	stringstream ss (stringstream::in | stringstream::out);
	
	ss<<prefix;
	ss<<"_";
	ss<<bus;
	ss>>buffer;

	return buffer;
}

void ProblemData::designCapacity(double minCapacityPerLine, double maxCapacityPerLine, double maxCapacity, int maxLineChanges ){

	char buffer[32];

	_allowCapacity = true;
	
	_maxCapacityPerLine = maxCapacityPerLine;
	_maxCapacity=maxCapacity;
	_maxLineChanges=maxLineChanges;
	_minCapacityPerLine= minCapacityPerLine;

	_designCapacity = IloNumVarArray(_env, _g->getBranchSize(), 0, maxCapacityPerLine);
	_budgetLinesUp = IloRangeArray(_env, _g->getBranchSize(), -IloInfinity, 0);
	_budgetLinesDown = IloRangeArray(_env, _g->getBranchSize(), -IloInfinity, 0);

	if(maxLineChanges>0){

	  _designLines = IloBoolVarArray(_env, _g->getBranchSize() );

	  for(int i=0; i<_g->getBranchSize(); i++){
	    _budgetLinesUp[i].setExpr( _designCapacity[i]-_maxCapacityPerLine*_designLines[i] );
	    _budgetLinesDown[i].setExpr( _minCapacityPerLine*_designLines[i] - _designCapacity[i] );
	    
	    _designCapacity[i].setName( getName("desCap",i,buffer) );
	    _designLines[i].setName( getName("desLine",i,buffer) );
	    
	    _budgetLinesUp[i].setName( getName("bdgtU",i,buffer) );
	    _budgetLinesDown[i].setName( getName("bdgtD",i,buffer) );

	  }

	  _budgetLines = IloRange(_env,0,_maxLineChanges);
	  _budgetLines.setExpr( IloSum(_designLines) );
	  _budgetLines.setName( "bdgtBool" );
	  
	  _model.add(_budgetLines);
	}

	else{
	  for(int i=0; i<_g->getBranchSize(); i++){
	    _budgetLinesUp[i].setExpr( _designCapacity[i]-_maxCapacityPerLine);
	    _budgetLinesDown[i].setExpr( _minCapacityPerLine - _designCapacity[i] );
	    
	    _budgetLinesUp[i].setName( getName("bdgtU",i,buffer) );
	    _budgetLinesDown[i].setName( getName("bdgtD",i,buffer) );
	    
	  }
	}

	
	_budgetCapacity = IloRange(_env,0,_maxCapacity);
	_budgetCapacity.setExpr( IloSum(_designCapacity) );

	_budgetCapacity.setName("bdgt");

	_model.add(_budgetCapacity);
	_model.add(_budgetLinesUp);

	//	_model.add(_budgetLinesDown);
}

void ProblemData::fixCapacity(double maxCapacity){
	_allowCapacity=true;

	_maxCapacity=maxCapacity;

	_designCapacity = IloNumVarArray(_env, _g->getBranchSize(), 0, 0);
	_budgetCapacity = IloRange(_env,0,maxCapacity);
	_budgetCapacity.setExpr( IloSum(_designCapacity) );
	const char name[5] = "bdgt";
	_budgetCapacity.setName(name);
	_model.add(_budgetCapacity);
}

ostream& operator << (ostream& os, ProblemData& pd){
	
	os<<"\nProblem Data: \n";
	os<<*pd._g<<"\n\n";
	os<<"Stages: "<<pd._stages<<", Outcomes: ";
	for(int i=0;i<pd._stages;i++){
	  os<<pd._outcomes[i]<<" ";
	}
	os<<", Scenarios: "<<pd._scenarios<<"\n";

	os<<"\nOutages \n";
	for(int i=0;i<pd._scenarios;i++){
		os<<"Scenario: "<<i+1<<" - ";
		for(int j=0;j<(int)pd._outages[i].size();j++){
			os<<pd._outages[i][j]<<"  ";
		}
		os<<"\n";
	}
	os<<"\nObjective Weights: \n";
	for(int i=0;i<(int)pd._objWeights.size();i++){
		os<<pd._objWeights[i]<<"  ";
	}
	os<<"\n\n";

	if(pd._allowReserve) os<<"Reserve Margin: "<<pd._reserveMargin<<"\n\n";

	if(pd._allowCapacity) {
		os<<"Expand Capacty\n";
		os<<"Min Capacity per Line: "<<pd._minCapacityPerLine<<"\n";
		os<<"Max Capacity per Line: "<<pd._maxCapacityPerLine<<"\n";
		os<<"Total Budget: "<<pd._maxCapacity<<"\n";
		if(pd._maxLineChanges>0) os<<"Max Line Changes: "<<pd._maxLineChanges<<"\n";
		else os<<"Max Line Changes: Unconstrained"<<endl;
	}

	os<<"\nLine Failures:\n";
	os<<"p: "<<pd._probability<<", L: "<<pd._effectiveDistributionL<<"\n"<<endl;

	os<<"Output File: "<<pd._outputFile<<endl;

	return os;
}
