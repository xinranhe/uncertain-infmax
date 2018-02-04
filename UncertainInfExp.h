#ifndef UNCERTAIN_INF_EXP
#define UNCERTAIN_INF_EXP

#include <string>
#include <vector>
using namespace std;

struct UncertainInfExp {
	string graphFilePath;
	string resultFilePath;
	int numRound;
	int numSim;
	int k;
	vector<double> detParameters;
	vector<double> weightParameters;
	bool isRelative;
	bool isDirected;

	// Model Type
	ModelType modelType;

	// flag for calculation
	bool isBoundMax;
	bool isBoundMin;
	bool isBoundObv;

	UncertainInfExp(): isDirected(true), modelType(IC) {}
	void runCascInfDiffMaxExperiment();
};

// Experiment for Section 5.3, Table 1
void runSimpleCliqueExp(int n, double edgeDet);

#endif