#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Graph.h"
#include "GraphManager.h"
#include "ReservoirManager.h"

class Algorithm {
	public:
		//adds an edge to the graph
		virtual void Add(const edge &e) = 0;
		//removes an edge from the graph
		virtual void Remove(const edge &e) = 0;
		
		// The query methods:
		// Finds the cluster index
		virtual int FindClusterIndex(vertex u) = 0;
		// finds all the verices in the cluster of u
		virtual vector<vertex> FindCluster(vertex u) = 0;
		// returns the number of clusters
		virtual int CountClusters() = 0;
};

// The classes that has to be implemented:
/*

class MetisAlgorithm {
public:
	void Add(const edge &e);
	void Remove(const edge &e);

	int FindClusterIndex(vertex u);
	vector<vertex> FindCluster(vertex u);
	int CountClusters();
};*/

class StructuralSampler : public Algorithm {
	private:
		StructuralReservoir strReservoir;
		SupportReservoir supReservoir;
		GraphManager manager;
	public:
		StructuralSampler(int maxClusterSize);
		virtual void Add(edge e);
		virtual void Remove(edge e);
		
		virtual int FindClusterIndex(vertex u)
			{ return manager.FindClusterIndex(u); }
		virtual vector<vertex> FindCluster(vertex u)
			{ return manager.FindCluster(u); }
		virtual int CountClusters()
			{ return manager.CountClusters(); }
};

#endif
