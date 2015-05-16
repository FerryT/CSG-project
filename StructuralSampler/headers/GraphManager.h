#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <unordered_map>
#include "Graph.h"

class GraphManager {
	private:
		// usage: the first integer is a node identifier. unordered_map<int,int> maintains a count to all the nodes it is connected to
		unordered_map<int, unordered_map<int,int> > graph;
		// stores the clusters. the first item of a cluster is the representative of this cluster
		vector< vector<vertex> > clusters;
		// the B value mentioned in the paper
		const int maxClusterSize;
		
		// reconstructs theClusters such that the elements of c1 and c2 are merged at the end of the vector
		void UnionClusters(int c1, int c2);
		// helperfunction to make adding edge function smaller
		void AddEdgeToGraph(const Edge &e);
		// deconstructs clusters at index c1 and c2 such that each vertex is in it's own cluster. If c1==c2 then only does this on cluster c1. Then, it reconstructs the clusters
		void RemakeClusters(int c1, int c2);
		// checks if any cluster after (and including) startCluster can/should be merged with any other cluster, since they are connected, and does so whenever two clusters are connected
		void MergeClusters(int startCluster);
	
	public:
		GraphManager(int max) : maxClusterSize(max) {}
		
		// just insert the edge into the graph
		void Add(const Edge &e);
		// remove exactly this edge (equal including p-value, only remove 1 instance)
		void RemoveExact(const Edge &e);
		// remove this edge (equal excluding p-value, only remove 1 instance)
		void Remove(const Edge &e);
		// if the 'constraint' mentioned in the paper is satisfied
		bool ConstraintSatisfied();
	
		// The query methods:
		// Finds the cluster index
		virtual int FindClusterIndex(vertex u);
		// finds all the verices in the cluster of u
		virtual vector<vertex> FindCluster(vertex u);
		// returns the number of clusters
		virtual int CountClusters();
};

#endif
