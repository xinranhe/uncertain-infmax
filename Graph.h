#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>
#include <random>
using namespace std;

#define MAXN 2000

struct Edge {
	int stId;
	int edId;
	double w;
	Edge(int _stId, int _edId, double _w) : stId(_stId), edId(_edId), w(_w) {}
	Edge() {}
};

struct Graph {
	// number of node
	int nodeNum;
	// number of edge
	int edgeNum;
	
	// storage for edges
	vector<vector<Edge> > inEdges;
	vector<vector<Edge> > outEdges;

	// constructor
	Graph(int _nodeNum): nodeNum(_nodeNum) {
		init();
	}
	Graph(): nodeNum(0) {
		init();
	}
	void init() {
		inEdges.resize(nodeNum);
		outEdges.resize(nodeNum);
	}

	// perturb the weight by absolute value or relative value
	void changeEdgeWeight(double det) {
		for(int i=0;i<nodeNum;i++) {
			for(int j=0;j<inEdges[i].size();j++) {
				inEdges[i][j].w = capping(inEdges[i][j].w + det);
			}
			for(int j=0;j<outEdges[i].size();j++) {
				outEdges[i][j].w = capping(outEdges[i][j].w + det);
			}
		}
	}
	// det shouble be in [-1,+1]
	void changeEdgeWeightRelative(double det) {
		for(int i=0;i<nodeNum;i++) {
			for(int j=0;j<inEdges[i].size();j++) {
				inEdges[i][j].w = capping(inEdges[i][j].w * (1 + det));
			}
			for(int j=0;j<outEdges[i].size();j++) {
				outEdges[i][j].w = capping(outEdges[i][j].w * (1 + det));
			}
		}
	}

	double capping(double value, double low = 0, double high = 1) {
		if(value > high) return high;
		if(value < low) return low;
		return value;
	}
	
	void addEdge(int st, int ed, double w);
	
	// load graph from file
	// format:
	// LINE 1      : n m (#node #edge)
	// LINE 2--m+1 : st ed w (start, end, weight)
	void readFromFile(const string& fileName, bool isDirected = true);
	// estimate cascade under IC model (openMP parallel)
	double estimateCascadeIC(const unordered_set<int>& seedNodes, int times);
	// estimate the difference in inf by adding newNode (deprecated)
	double estimateDetCascIC(const unordered_set<int>& seedNodes, int newNode, int times) {
		unordered_set<int> tempSeed = seedNodes;
		double originalResult = estimateCascadeIC(tempSeed, times);
		tempSeed.insert(newNode);
		double newResult = estimateCascadeIC(tempSeed, times);
		return newResult - originalResult;
	}
	// fast method for estimating the difference in inf by adding newNode
	double estimateDetCascICFast(const unordered_set<int>& seedNodes, int newNode, int times);

	// similar function for LT model
	double estimateCascadeLT(const unordered_set<int>& seedNodes, int times);
	double estimateDetCascLT(const unordered_set<int>& seedNodes, int newNode, int times) {
		unordered_set<int> tempSeed = seedNodes;
		double originalResult = estimateCascadeLT(tempSeed, times);
		tempSeed.insert(newNode);
		double newResult = estimateCascadeLT(tempSeed, times);
		return newResult - originalResult;
	}
	double estimateDetCascLTFast(const unordered_set<int>& seedNodes, int newNode, int times);
};


#endif