#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <unordered_map>
#include "Graph.h"

class graphManager{
private:
	void updateUnionFind();//updates the unionfind datastructure
public:
	//vector<edge> theGraph;
	unordered_map<int, unordered_map<int,int>> theGraph;//usage: the first integer is a node identifier. unordered_map<int,int> maintains a count to all the nodes it is connected to
	graphManager();
	void insertEdge(edge e);//just insert the edge into the graph
	void removeEdgeExact(edge e);//remove exactly this edge (equal including p-value, only remove 1 instance)
	void removeEdge(edge e);//remove this edge (equal excluding p-value, only remove 1 instance)
	bool constraintSatisfied();//if the 'constraint' mentioned in the paper is satisfied

	//The query methods:
	//Finds the cluster index
	virtual int FindClusterIndex(vertex u) = 0;
	//finds all the verices in the cluster of u
	virtual vector<vertex> FindCluster(vertex u) = 0;
	//returns the number of clusters
	virtual int CountClusters() = 0;
};

#endif