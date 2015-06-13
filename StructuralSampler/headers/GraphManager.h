#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include "Graph.h"

class GraphManager {
	public:
		// the B value mentioned in the paper
		const clusterid maxClusterSize;
		
		GraphManager(clusterid max = 10);
		~GraphManager();
		
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
		virtual clusterid FindClusterIndex(vertex u);
		// finds all the verices in the cluster of u
		virtual Vertices FindCluster(vertex u);
		// returns the number of clusters
		virtual clusterid CountClusters();
		// returns the vertices in the cluster with index i
		virtual Vertices GetCluster(clusterid i);
	private:
		struct Data;
		Data *data;
};

#endif
