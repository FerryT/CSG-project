#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <unordered_map>
#include "Graph.h"
#include "GraphManager.h"

class Algorithm {
	public:
		//adds an edge to the graph
		virtual void Add(edge e) = 0;
		//removes an edge from the graph
		virtual void Remove(edge e) = 0;

		//The query methods:
		//Finds the cluster index
		virtual int FindClusterIndex(vertex u) = 0;
		//finds all the verices in the cluster of u
		virtual vector<vertex> FindCluster(vertex u) = 0;
		//returns the number of clusters
		virtual int CountClusters() = 0;
};

// The classes that has to be implemented:
/*

class MetisAlgorithm {
public:
	void Add(edge e);
	void Remove(edge e);

	int FindClusterIndex(vertex u);
	vector<vertex> FindCluster(vertex u);
	int CountClusters();
};*/

class StructuralSampler {
public:
	StructuralSampler();
	virtual void Add(edge newEdge, structuralReservoir strReservoir, supportReservoir supReservoir, graphManager manager);
	virtual void Remove(edge theEdge, structuralReservoir strReservoir, supportReservoir supReservoir, graphManager manager);

	virtual int FindClusterIndex(vertex u);
	virtual vector<vertex> FindCluster(vertex u);
	virtual int CountClusters();
};

#endif