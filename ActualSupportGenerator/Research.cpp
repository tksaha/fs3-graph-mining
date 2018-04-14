//============================================================================
// Name        : Research.cpp
// Author      : Tanay
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>

#include "randomMining.h"

//using namespace std;




int main(int argc, char *argv[]) {
		randomMining *rm;
		rm=new randomMining();
		rm->parseArgs(argc,argv);
		rm->ProcessInputFile();
		rm->printSupportList();
		rm->randomMiner();
		//delete rm;
		//cout<<"Program successfully terminated"<<endl;
		return 0;
}
