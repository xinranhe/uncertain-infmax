#ifndef INF_MAXIMIZER
#define INF_MAXIMIZER

#include "Graph.h"

#include <unordered_set>
#include <vector>
using namespace std;

enum ModelType{IC=0, LT=1};

// use random greedy to maximize Inf Diff
double maximizeInfDiff(Graph& gMax, Graph& gMin, int k, unordered_set<int>& resultSet, int numSim, int numRounds,vector<double>* accResults = NULL , ModelType modelType = IC);
// use random greedy to maximize Inf Diff with CELF optimization
double maximizeInfDiffFast(Graph& gMax, Graph& gMin, int k, unordered_set<int>& resultSet, int numSim, int numRounds, vector<double>* accResults = NULL, ModelType modelType = IC);

// use greedy to maximize the Inf
double maximizeInf(Graph& g, int k, unordered_set<int>& resultSet, int numSim, ModelType modelType = IC);
// use greedy to maximize the Inf with CELF optimization
double maximizeInfFast(Graph& g, int k, unordered_set<int>& resultSet, int numSim, vector<double>* accResults = NULL, ModelType modelType = IC);

// help functions
double estimateInfDiff(Graph& gMax, Graph& gMin, unordered_set<int>& seedSet, int numSim, ModelType modelType = IC);
double estimateInfDiffDet(Graph& gMax, Graph& gMin, unordered_set<int>& seedSet, int numSim, int newNode, ModelType modelType = IC);

#endif