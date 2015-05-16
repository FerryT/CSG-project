#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <unordered_map>
#include "Graph.h"

class GraphManager {
	private:
		//usage: the first integer is a node identifier. unordered_map<int,int> maintains a count to all the nodes it is connected to
		unordered_map<int, unordered_map<int,int> > g;
		//stores the clusters. the first item of a cluster is the representative of this cluster
		vector< vector<vertex> > clusters;
		
		void UnionClusters(int c1, int c2);
		void AddEdgeToGraph(const edge &e);
		void RemakeClusters(int c1, int c2);
		void MergeClusters();
	
	public:
		// just insert the edge into the graph
		void Add(const edge &e);
		// remove exactly this edge (equal including p-value, only remove 1 instance)
		void RemoveExact(const edge &e);
		// remove this edge (equal excluding p-value, only remove 1 instance)
		void Remove(const edge &e);
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
