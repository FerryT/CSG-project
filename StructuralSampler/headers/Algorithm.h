#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "GraphInput.h"
#include "Graph.h"
#include "GraphManager.h"
#include "ReservoirManager.h"

//------------------------------------------------------------------------------

class Algorithm: public Output {
	public:
		// The query methods:
		// Finds the cluster index
		virtual int FindClusterIndex(vertex u) = 0;
		// finds all the verices in the cluster of u
		virtual vector<vertex> FindCluster(vertex u) = 0;
		// returns the number of clusters
		virtual int CountClusters() = 0;
};

//------------------------------------------------------------------------------

// The classes that has to be implemented:
/*

class MetisAlgorithm {
public:
	void Add(const Edge &e);
	void Remove(const Edge &e);

	int FindClusterIndex(vertex u);
	vector<vertex> FindCluster(vertex u);
	int CountClusters();
};*/

//------------------------------------------------------------------------------

class StructuralSampler : public Algorithm {
	private:
		StructuralReservoir strReservoir;
		SupportReservoir supReservoir;
		GraphManager manager;
	public:
		StructuralSampler(int maxClusterSize);
		virtual void Add(Edge e);
		virtual void Remove(Edge e);
		
		virtual int FindClusterIndex(vertex u)
			{ return manager.FindClusterIndex(u); }
		virtual vector<vertex> FindCluster(vertex u)
			{ return manager.FindCluster(u); }
		virtual int CountClusters()
			{ return manager.CountClusters(); }
};

//------------------------------------------------------------------------------

#endif // ALGORITHM_H
