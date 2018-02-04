#include "InfMaximizer.h"

#include <queue>
#include <unordered_set>
using namespace std;

struct QueueNode {
	int id;
	double weight;
	int lastUpdate;
	bool operator < (const QueueNode& other) const {
		return weight < other.weight;
	}
	QueueNode(int _id, double _value, int _update): id(_id), weight(_value), lastUpdate(_update) {}
	QueueNode() {}
};


struct TempNode {
	int id;
	double weight;
	TempNode(int _id, double _weight):id(_id), weight(_weight) {}
	bool operator < (const TempNode& other) const {
		return weight > other.weight;
	}
};


// evaluate g(S) = \sigma_max(S) - \sigma_min(S)
double estimateInfDiff(Graph& gMax, Graph& gMin, unordered_set<int>& seedSet, int numSim, ModelType modelType) {
	if(modelType == IC) {
		return gMax.estimateCascadeIC(seedSet, numSim) - gMin.estimateCascadeIC(seedSet, numSim);
	} else 
	{
		return gMax.estimateCascadeLT(seedSet, numSim) - gMin.estimateCascadeLT(seedSet, numSim);
	}
}
// evaluate g(SU{v}) = (\sigma_max(SU{v}) - \sigma_min(SU{v})) - (\sigma_max(S) - \sigma_min(S))
//                   = (\sigma_max(SU{v}) - \sigma_max(S)) - (\sigma_min(SU{v}) - \sigma_min(S))
double estimateInfDiffDet(Graph& gMax, Graph& gMin, unordered_set<int>& seedSet, int newNode, int numSim ,ModelType modelType) {
	if(modelType == IC) {
		return gMax.estimateDetCascICFast(seedSet, newNode, numSim) - gMin.estimateDetCascICFast(seedSet, newNode, numSim);
	} else {
		return gMax.estimateDetCascLTFast(seedSet, newNode, numSim) - gMin.estimateDetCascLTFast(seedSet, newNode, numSim);
	}
}



double maximizeInfDiff(Graph& gMax, Graph& gMin, int k, unordered_set<int>& resultSet, int numSim, int numRounds, vector<double>* accResults, ModelType modelType) {
	double bestValue = 0;
	unordered_set<int> bestSet;
	
	if(accResults) {
		accResults->clear();
		accResults->push_back(0);
	}
	
	cout << "Inf Diff Maximization:";
	for(int t=1;t<=numRounds;t++) {
		
		cout << "Round:" << t <<" ;Iteration: ";
		unordered_set<int> currentSet;
		
		vector<double> tempAccResult;
		tempAccResult.push_back(0);

		for(int i=1;i<=k;i++) {
			cout << i << " ";
			priority_queue<TempNode> myQueue;

			for(int nId=0;nId<gMax.nodeNum;nId++) {
				if(currentSet.find(nId)!=currentSet.end()) {
					continue;
				}
				// evaluate value
				double funcValue = estimateInfDiffDet(gMax, gMin, currentSet, nId, numSim, modelType);
				if(myQueue.size()< k) {
					myQueue.push(TempNode(nId, funcValue));
				} else {
					if(funcValue > myQueue.top().weight) {
						myQueue.pop();
						myQueue.push(TempNode(nId, funcValue));
					}
				}
			}
			// random draw an item
			vector<int> nodeIds;
			nodeIds.resize(k);
			int p =0;
			while(!myQueue.empty()) {
				nodeIds[p++] = myQueue.top().id;
				myQueue.pop();
			}
			int randPos = rand()%k;
			currentSet.insert(nodeIds[randPos]);

			if(accResults) {
				tempAccResult.push_back(estimateInfDiff(gMax, gMin, currentSet, numSim));
			}
		}
		
		double currentValue = estimateInfDiff(gMax, gMin, currentSet, numSim, modelType);
		cout << " CurrentVale=" << currentValue << " Current set:" ;
		for(auto it = currentSet.begin(); it!=currentSet.end(); it++) {
			cout << (*it) << " ";
		}
		cout<<endl;

		if(currentValue > bestValue) {
			bestValue = currentValue;
			bestSet = currentSet;
			if(accResults) {
				*accResults = tempAccResult;
			}
		}
	}
	resultSet = bestSet;
	cout << "Finished... BestVale=" << bestValue << " Best set:" ;
	for(auto it = bestSet.begin(); it!=bestSet.end(); it++) {
		cout << (*it) << " ";
	}
	cout<<endl;
	return bestValue;
}

double maximizeInfDiffFast(Graph& gMax, Graph& gMin, int k, unordered_set<int>& resultSet, int numSim, int numRounds, vector<double>* accResults, ModelType modelType) {
	double bestValue = 0;
	unordered_set<int> bestSet;

	if(accResults) {
		accResults->clear();
		accResults->push_back(0);
	}
	cout << "====Inf Diff Maximization (CELF):====" << endl;
	
	vector<QueueNode> initNodes;
	cout << "Init nodes:";
	for(int i=0;i<gMax.nodeNum;i++) {
		cout << i << " ";
		unordered_set<int> nullSet;
		double tempW = estimateInfDiffDet(gMax, gMin, nullSet, i, numSim, modelType);
		initNodes.push_back(QueueNode(i, tempW, 1));
	}
	cout << endl;

	for(int t=1;t<=numRounds;t++) {
		cout << "Round:" << t << endl;;
		unordered_set<int> currentSet;

		priority_queue<QueueNode> myQueue;
		// Initialization
		for(int i=0;i<initNodes.size();i++) {
			myQueue.push(initNodes[i]);
		}

		vector<double> tempAccResult;
		tempAccResult.push_back(0);

		cout << "Iteration:";
		for(int i=1;i<=k;i++) {
			cout << i << " ";
			vector<QueueNode> selectedNodes;
			while(selectedNodes.size() < k) {
				while(true) {
					QueueNode tempNode = myQueue.top();
					myQueue.pop();
					if(tempNode.lastUpdate == i) {
						selectedNodes.push_back(tempNode);
						break;
					} else {
						tempNode.weight = estimateInfDiffDet(gMax, gMin, currentSet, tempNode.id, numSim, modelType);
						tempNode.lastUpdate = i;
						myQueue.push(tempNode);
					}
				}
			}
			int randId = rand()%k;
			int selectedId = selectedNodes[randId].id;
			// push all the not selected nodes into the queue
			for(int j=0;j<selectedNodes.size();j++) {
				if(j!=randId) {
					myQueue.push(selectedNodes[j]);
				}
			}
			currentSet.insert(selectedId);
			if(accResults) {
				tempAccResult.push_back(estimateInfDiff(gMax, gMin, currentSet, numSim));
			}
		}
		cout << endl;
		double currentValue = estimateInfDiff(gMax, gMin, currentSet, numSim, modelType);
		if(currentValue > bestValue) {
			bestValue = currentValue;
			bestSet = currentSet;
			if(accResults) {
				*accResults = tempAccResult;
			}
		}
	}
	resultSet = bestSet;
	cout << "Finished... BestVale=" << bestValue << " Best set:" ;
	for(auto it = bestSet.begin(); it!=bestSet.end(); it++) {
		cout << (*it) << " ";
	}
	cout<<endl;
	return bestValue;
}

double maximizeInf(Graph& g, int k, unordered_set<int>& resultSet, int numSim, ModelType modelType) {
	cout << "Inf Maximization:" << endl;
	resultSet.clear();
	double resultValue = 0;
	for(int t=1;t<=k;t++) {
		cout<<"Iteration: " << t << "; ";
		int bestNode = -1;
		double bestGain = -1;
		for(int i=0;i<g.nodeNum;i++) {
			cout << i << " ";
			if(resultSet.find(i)!=resultSet.end()) {
				continue;
			}
			double tempGain;
			if(modelType==IC) {
				tempGain = g.estimateDetCascICFast(resultSet, i , numSim);
			} else {
				tempGain = g.estimateDetCascLTFast(resultSet, i , numSim);
			}
			if(tempGain > bestGain) {
				bestGain = tempGain;
				bestNode = i;
			}
		}
		cout << endl;
		resultSet.insert(bestNode);
		resultValue += bestGain;
	}
	return resultValue;
}

double maximizeInfFast(Graph& g, int k, unordered_set<int>& resultSet, int numSim, vector<double>* accResults, ModelType modelType) {
	cout << "====Inf Maximization (CELF):====" << endl;
	resultSet.clear();
	if(accResults) {
		accResults->clear();
		accResults->push_back(0);
	}
	// init all nodes
	cout << "Initialization with nodes:";
	priority_queue<QueueNode> myQueue;
	for(int i=0;i<g.nodeNum;i++) {
		cout << i << " ";
		cout.flush();
		unordered_set<int> nullSet;
		double tempW = 0;
		if(modelType==IC) {
			tempW = g.estimateDetCascIC(nullSet, i , numSim);
		} else {
			tempW = g.estimateDetCascLT(nullSet, i, numSim);
		}
		myQueue.push(QueueNode(i, tempW, 1));
	}
	cout << endl;
	// iterations to pick seed
	cout << "Iteration:";
	for(int t=1;t<=k;t++) {
		cout << t << " ";
		while(true)
		{
			QueueNode tempNode = myQueue.top();
			myQueue.pop();
			if(tempNode.lastUpdate == t) {
				resultSet.insert(tempNode.id);
				if(accResults) {
					accResults->push_back(g.estimateCascadeIC(resultSet, numSim));
				}
				break;
			} else {
				if(modelType==IC) {
					tempNode.weight = g.estimateDetCascICFast(resultSet, tempNode.id, numSim);
				} else {
					tempNode.weight = g.estimateDetCascLTFast(resultSet, tempNode.id, numSim);
				}
				tempNode.lastUpdate = t;
				myQueue.push(tempNode);
			}
		}
	}
	cout << endl;
	double resultValue = g.estimateCascadeIC(resultSet, numSim);
	cout << "Finished... BestValue=" << resultValue << endl;
	return resultValue;
}