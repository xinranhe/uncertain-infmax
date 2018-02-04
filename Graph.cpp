#include "Graph.h"

#include <fstream>
#include <iostream>
#include <omp.h>
#include <queue>
#include <unordered_set>
using namespace std;


void Graph::addEdge(int st, int ed, double w) {
	inEdges[ed].push_back(Edge(st,ed,w));
	outEdges[st].push_back(Edge(st,ed,w));
}

void Graph::readFromFile(const string& fileName, bool isDirected) {
	ifstream fin;
	fin.open(fileName);
	fin >> nodeNum >> edgeNum;
	init();
	int st,ed;
	double weight;
	for(int i=0;i<edgeNum;i++) {
		fin >> st >> ed >> weight;
		addEdge(st,ed,weight);
		if(!isDirected) {
			addEdge(ed,st,weight);
		}
	}
}

double Graph::estimateCascadeIC(const unordered_set<int>& seedNodes, int times) {
	
	double result = 0;
#pragma omp parallel for reduction(+:result)
	for(int t = 1;t<=times;t++) {

		bool isActivated[MAXN];
		for(int i=0;i<nodeNum; i++) isActivated[i] = false;

		result += seedNodes.size();
		queue<int> myQuene;

		for(auto it = seedNodes.begin(); it!=seedNodes.end(); ++it) {
			myQuene.push(*it);
			isActivated[*it] = true;
		}
		while(!myQuene.empty()) {
			int st = myQuene.front();
			myQuene.pop();
			for(auto it=outEdges[st].begin(); it!=outEdges[st].end(); ++it) {
				int ed = (*it).edId;
				double weight = (*it).w;
				if(isActivated[ed]) {
					continue;
				}
				bool tempActivated = (double(rand())/RAND_MAX < weight);
				if(tempActivated) {
					myQuene.push(ed);
					isActivated[ed] = true;
					result++;
				}
			}
		}
	}
	return result / times;
}

double Graph::estimateDetCascICFast(const unordered_set<int>& seedNodes, int newNode, int times) {
	double result = 0;

#pragma omp parallel for reduction(+:result)
	for(int t = 1;t<=times; t++) {
		bool isActivated[MAXN];
		for(int i=0;i<nodeNum; i++) isActivated[i] = false;
		// activate already seeds
		queue<int> myQuene;
		for(auto it = seedNodes.begin(); it!=seedNodes.end(); ++it) {
			myQuene.push(*it);
			isActivated[*it] = true;
		}
		// cascade on seed set S
		while(!myQuene.empty()) {
			int st = myQuene.front();
			myQuene.pop();
			for(auto it=outEdges[st].begin(); it!=outEdges[st].end(); ++it) {
				int ed = (*it).edId;
				double weight = (*it).w;
				if(isActivated[ed]) {
					continue;
				}
				bool tempActivated = (double(rand())/RAND_MAX < weight);
				if(tempActivated) {
					myQuene.push(ed);
					isActivated[ed] = true;
				}
			}
		}
		// activation of new node 
		if(isActivated[newNode]) {
			continue;
		} else {
			result ++; // activate of the new node
			isActivated[newNode] = true;
			myQuene.push(newNode); // cascade from the new node
			while(!myQuene.empty()) {
				int st = myQuene.front();
				myQuene.pop();
				for(auto it=outEdges[st].begin(); it!=outEdges[st].end(); ++it) {
					int ed = (*it).edId;
					double weight = (*it).w;
					if(isActivated[ed]) {
						continue;
					}
					bool tempActivated = (double(rand())/RAND_MAX < weight);
					if(tempActivated) {
						myQuene.push(ed);
						isActivated[ed] = true;
						result++;
					}
				}
			}
		}
	}
	return result / times;
}

double Graph::estimateCascadeLT(const unordered_set<int>& seedNodes, int times) {
	double result = 0;

#pragma omp parallel for reduction(+:result)
	for(int t=1;t<=times;t++) {
		bool isActivated[MAXN];
		double threshold[MAXN];
		double currentWeight[MAXN];
		// generate threshold
		for(int i=0;i<nodeNum;i++) {
			isActivated[i] = false;
			threshold[i] = double(rand())/RAND_MAX;
			currentWeight[i] = 0;
		}
		queue<int> activatedNodes;
		for(auto it= seedNodes.begin(); it!=seedNodes.end(); ++it) {
			int st = (*it);
			activatedNodes.push(st);
			isActivated[st] = true;
			result++;
		}
		while(!activatedNodes.empty()) {
			int st = activatedNodes.front();
			activatedNodes.pop();
			for(auto it = outEdges[st].begin(); it!=outEdges[st].end(); ++it) {
				int ed = (*it).edId;
				if(isActivated[ed]) {
					continue;
				}
				currentWeight[ed] += (*it).w;
				if(currentWeight[ed] >= threshold[ed]) {
					isActivated[ed] = true;
					activatedNodes.push(ed);
					result++;
				}
			}
		}
	}
	return result / times;
}

double Graph::estimateDetCascLTFast(const unordered_set<int>& seedNodes, int newNode, int times) {
	double result = 0;

#pragma omp parallel for reduction(+:result)
	for(int t=1;t<=times;t++) {
		bool isActivated[MAXN];
		double threshold[MAXN];
		double currentWeight[MAXN];
		// generate threshold
		for(int i=0;i<nodeNum;i++) {
			isActivated[i] = false;
			threshold[i] = double(rand())/RAND_MAX;
			currentWeight[i] = 0;
		}
		queue<int> activatedNodes;
		// activate already seeds
		for(auto it= seedNodes.begin(); it!=seedNodes.end(); ++it) {
			int st = (*it);
			activatedNodes.push(st);
			isActivated[st] = true;
		}
		while(!activatedNodes.empty()) {
			int st = activatedNodes.front();
			activatedNodes.pop();
			for(auto it = outEdges[st].begin(); it!=outEdges[st].end(); ++it) {
				int ed = (*it).edId;
				if(isActivated[ed]) {
					continue;
				}
				currentWeight[ed] += (*it).w;
				if(currentWeight[ed] >= threshold[ed]) {
					isActivated[ed] = true;
					activatedNodes.push(ed);
				}
			}
		}
		// activation of the new seed
		if(isActivated[newNode]) {
			continue;
		} else {
			result++;
			activatedNodes.push(newNode);
			isActivated[newNode] = true;
			// cascade from the new node
			while(!activatedNodes.empty()) {
				int st = activatedNodes.front();
				activatedNodes.pop();
				for(auto it = outEdges[st].begin(); it!=outEdges[st].end(); ++it) {
					int ed = (*it).edId;
					if(isActivated[ed]) {
						continue;
					}
					currentWeight[ed] += (*it).w;
					if(currentWeight[ed] >= threshold[ed]) {
						isActivated[ed] = true;
						activatedNodes.push(ed);
						result++;
					}
				}
			}
		}
	}
	return result / times;
}