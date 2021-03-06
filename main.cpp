#include "Graph.h"
#include "InfMaximizer.h"
#include "UncertainInfExp.h"
#include "UtilFunc.h"

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <sstream>
#include <string>
#include <unordered_set>
using namespace std;

#define IS_LOG_TO_FILE true

string kGraphFolder = "./graph/";
string kResultFolder = "./result/";
string kLogFolder = "./log/";

void init()
{
	srand((unsigned int)time(0));
	kGraphFolder = "./graph/";
	kResultFolder = "./result/";
	kLogFolder = "./log/";

	if(IS_LOG_TO_FILE) {
		string logFile = kLogFolder + "log.txt";
		freopen(logFile.c_str(), "w", stdout);
	}
	// setting for openmp
	omp_set_num_threads(10);
	omp_set_dynamic(0);
}

void mainRunICRegExp(int d) {
	ostringstream s;
	s << "reg_" << d;
	
	string mainGraphFile = s.str() + ".graph";
	string mainResultFile = s.str() + ".result";
	double p = 1.0 / double(d);
	
	UncertainInfExp mainExp;
	mainExp.graphFilePath = kGraphFolder + mainGraphFile;
	mainExp.resultFilePath = kResultFolder + mainResultFile;
	mainExp.k = 20;
	mainExp.numSim = 500;
	mainExp.numRound = 5;
	mainExp.isRelative = true;
	mainExp.isDirected = false;

	mainExp.isBoundMin = true;
	mainExp.isBoundMax = true;
	mainExp.isBoundObv = true;

	mainExp.weightParameters.clear();
	for(int i=-2;i<=2;i++) {
		double a = i * 0.2;
		mainExp.weightParameters.push_back( p * (1+a));
	}

	mainExp.detParameters.clear();
	mainExp.detParameters.push_back(0.01);
	mainExp.detParameters.push_back(0.05);
	mainExp.detParameters.push_back(0.1);
	mainExp.detParameters.push_back(0.2);
	mainExp.detParameters.push_back(0.5);

	mainExp.runCascInfDiffMaxExperiment();
}

void mainRunICDetExp(string graphName) {
	string mainGraphFile = graphName + ".graph";
	string mainResultFile = graphName + "_getAll_NoCELF.result";
	
	UncertainInfExp mainExp;
	mainExp.graphFilePath = kGraphFolder + mainGraphFile;
	mainExp.resultFilePath = kResultFolder + mainResultFile;
	mainExp.k = 20;
	mainExp.numSim = 500;
	mainExp.numRound = 5;
	mainExp.isRelative = true;
	mainExp.isDirected = false;

	mainExp.isBoundMin = true;
	mainExp.isBoundMax = true;
	mainExp.isBoundObv = true;

	mainExp.weightParameters.clear();
	mainExp.weightParameters.push_back(0.01);
	mainExp.weightParameters.push_back(0.02);
	mainExp.weightParameters.push_back(0.05);
	mainExp.weightParameters.push_back(0.1);

	mainExp.detParameters.clear();
	mainExp.detParameters.push_back(0.01);
	mainExp.detParameters.push_back(0.05);
	mainExp.detParameters.push_back(0.1);
	mainExp.detParameters.push_back(0.2);
	mainExp.detParameters.push_back(0.5);

	mainExp.runCascInfDiffMaxExperiment();
}

int main(int argc, char** argv) {
	init();

	string expType = string(argv[1]);
	if(expType == "Reg") {
		int d = atoi(argv[2]);
		mainRunICRegExp(d);
	} else if(expType == "ICDet") {
		string graphName = string(argv[2]);
		mainRunICDetExp(graphName);
	}
	return 0;
}
