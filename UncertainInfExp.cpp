#include "Graph.h"
#include "InfMaximizer.h"
#include "UncertainInfExp.h"
#include "UtilFunc.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

void UncertainInfExp::runCascInfDiffMaxExperiment() {
	// load graph
	Graph originGraph;
	originGraph.readFromFile(graphFilePath, isDirected);

	// open result file
	ofstream fout;
	fout.open(resultFilePath.c_str());

	// run Inf maximization
	vector<double> accInfMax, accInfMin, accInfObv;
	vector<double> accDetMaxMin, accDetMaxObv;
	unordered_set<int> detMaxMinSet, detMaxObvSet; 
	unordered_set<int> infMaxSet, infMinSet, infObvSet;

	for(int ei = 0; ei < weightParameters.size(); ei++) {

		double edgeWeight = weightParameters[ei];
		Graph expGraph = originGraph;
		// the count of parallel edges is given in original graph 
		// the graph edge weight used is: edgeWeight * count 
		expGraph.changeEdgeWeightRelative(edgeWeight - 1);
		
		fout << "====Start new base graph====" << endl;
		fout << "edgeWeight=" << edgeWeight << endl;
		
		// clear all current results
		accInfObv.clear(); infObvSet.clear();

		// Influence maximization
		if(isBoundObv) {
			maximizeInfFast(expGraph, k, infObvSet, numSim, &accInfObv, modelType);
			fout << "====InfMax:Obv====" << endl;
			fout << "Values: ";
			for(int i=0;i<accInfObv.size();i++) {
				fout << accInfObv[i] << " ";
			}
			fout << endl;
			fout << "Set: " << intSetToStr(infObvSet) << endl;
		}

		for(int di = 0; di < detParameters.size(); di++) {
			double edgeDet = detParameters[di];

			fout << "====Start new det graph====" << endl;
			fout << "edgeDet=" << edgeDet << endl;

			// clear all det results
			accInfMax.clear(); accInfMin.clear(); 
			infMaxSet.clear(); infMinSet.clear();

			accDetMaxMin.clear(); accDetMaxObv.clear();
			detMaxMinSet.clear(); detMaxObvSet.clear();

			// generate two graphs
			Graph maxGraph, minGraph;
			maxGraph = expGraph;
			minGraph = expGraph;
			if(isRelative) {
				maxGraph.changeEdgeWeightRelative(edgeDet);
				minGraph.changeEdgeWeightRelative(-edgeDet);
			} else {
				maxGraph.changeEdgeWeight(edgeDet);
				minGraph.changeEdgeWeight(-edgeDet);
			}

			if(isBoundMax) {
				maximizeInfFast(maxGraph, k, infMaxSet, numSim, &accInfMax, modelType);
				fout << "====InfMax:Max====" << endl;
				fout << "Values: ";
				for(int i=0;i<accInfMax.size();i++) {
					fout << accInfMax[i] << " ";
				}
				fout << endl;
				fout << "Set: " << intSetToStr(infMaxSet) << endl;

				// empirical different between max and min graph
				double diffMaxMin = estimateInfDiff(maxGraph, minGraph, infMaxSet, numSim, modelType);
				fout << "InfDiffMaxMin=" << diffMaxMin << endl;
			}
			if(isBoundObv) {
				maximizeInfDiff(maxGraph, expGraph, k, detMaxObvSet, numSim, numRound, &accDetMaxObv, modelType);
				fout << "====InfDiffMax:MaxObv====" << endl;
				fout << "Values: ";
				for(int i=0;i<accDetMaxObv.size();i++) {
					fout << accDetMaxObv[i] << " ";
				}
				fout << endl;
				fout << "Set: " << intSetToStr(detMaxObvSet) << endl;

				double diffObvMin = estimateInfDiff(expGraph, minGraph, infMaxSet, numSim, modelType);
				fout << "InfDiffObvMin=" << diffObvMin << endl;
			}
			if(isBoundMin) {
				bool isCalDetInf = true;
				if(isCalDetInf) {
					maximizeInfDiff(maxGraph, minGraph, k, detMaxMinSet, numSim, numRound, &accDetMaxMin, modelType);
					fout << "====InfDiffMax:MaxMin====" << endl;
					fout << "Values: ";
					for(int i=0;i<accDetMaxMin.size();i++) {
						fout << accDetMaxMin[i] << " ";
					}
					fout << endl;
					fout << "Set: " << intSetToStr(detMaxMinSet) << endl;
				}
			}
		}
	}
}

void runSimpleCliqueExp(int n, double _detWeight) {
	double edgeWeight = 1.0/double(n);
	double detWeight = _detWeight/double(n);
	// generate clique graph
	Graph expGraph(n);
	for(int st=0;st<n;st++) {
		for(int ed=0;ed<st;ed++) {
			// undirected clique
			expGraph.addEdge(st, ed, edgeWeight);
			expGraph.addEdge(ed, st, edgeWeight);
		}
	}
	unordered_set<int> seedSet;
	seedSet.insert(0);

	Graph maxGraph, minGraph;
	maxGraph = expGraph;
	minGraph = expGraph;
	maxGraph.changeEdgeWeight(edgeWeight);
	minGraph.changeEdgeWeight(-edgeWeight);

	int times = 100;

	cout << "Max graph:" << maxGraph.estimateCascadeIC(seedSet, times) << endl;
	cout << "Min graph:" << minGraph.estimateCascadeIC(seedSet, times) << endl;
}
