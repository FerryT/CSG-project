#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <unordered_map>
#include "Graph.h"

class graphManager{
private:
	vector< vector<vertex> > theClusters;//stores the clusters. the first item of a cluster is the representative of this cluster
	void UnionClusters(int c1, int c2);//reconstructs theClusters such that the elements of c1 and c2 are merged at the end of the vector
	void AddEdgeToGraph(edge e);//helperfunction to make adding edge function smaller
	void remakeClusters(int c1, int c2);//deconstructs clusters at index c1 and c2 such that each vertex is in it's own cluster. If c1==c2 then only does this on cluster c1. Then, it reconstructs the clusters
	void mergeClusters(int startCluster);//checks if any cluster after (and including) startCluster can/should be merged with any other cluster, since they are connected, and does so whenever two clusters are connected
	int maxClusterSize;//the B value mentioned in the paper
public:
	unordered_map<int, unordered_map<int,int>> theGraph;//usage: the first integer is a node identifier. unordered_map<int,int> maintains a count to all the nodes it is connected to
	graphManager(int B);
	void insertEdge(edge e);//just insert the edge into the graph
	void removeEdgeExact(edge e);//remove exactly this edge (equal including p-value, only remove 1 instance)
	void removeEdge(edge e);//remove this edge (equal excluding p-value, only remove 1 instance)
	bool constraintSatisfied();//if the 'constraint' mentioned in the paper is satisfied

	//The query methods:
	//Finds the cluster index
	int FindClusterIndex(vertex u);
	//finds all the verices in the cluster of u
	vector<vertex> FindCluster(vertex u);
	//returns the number of clusters
	int CountClusters();
};

#endif