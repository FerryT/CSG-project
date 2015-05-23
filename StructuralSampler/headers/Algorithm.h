#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "GraphInput.h"
#include "Graph.h"
#include "GraphManager.h"
#include "ReservoirManager.h"
#include <iostream>

//------------------------------------------------------------------------------

class Algorithm: public Output {
	public:
		virtual void Add(Edge e) = 0;
		virtual void Remove(Edge e) = 0;

		// The query methods:
		// Finds the cluster index
		virtual int FindClusterIndex(vertex u) = 0;
		// finds all the verices in the cluster of u
		virtual vector<vertex> FindCluster(vertex u) = 0;
		// returns the number of clusters
		virtual int CountClusters() = 0;
		// gets all vertices of cluster
		virtual vector<vertex> GetCluster(int index) = 0;

		//parses arguments for configuration
		virtual void ParseArguments(const vector<string>& arguments) { std::cout << "arguments for this algorithm are not yet parsed" << std::endl; }
};

//------------------------------------------------------------------------------

class Metis : public Algorithm {
	public:
		Metis(int numClusters);
		virtual ~Metis();

		virtual void Add(Edge e);
		virtual void Remove(Edge e);

		virtual int FindClusterIndex(vertex u);
		virtual vector<vertex> FindCluster(vertex u);
		virtual int CountClusters();
		virtual vector<vertex> GetCluster(int index);
	private:
		struct Data;
		Data *data;
};

//------------------------------------------------------------------------------

class StructuralSampler : public Algorithm {
	private:
		StructuralReservoir strReservoir;
		SupportReservoir supReservoir;
		GraphManager manager;
	public:
		StructuralSampler();
		StructuralSampler(int maxClusterSize);
		
		virtual void Add(Edge e);
		virtual void Remove(Edge e);
		
		virtual int FindClusterIndex(vertex u)
			{ return manager.FindClusterIndex(u); }
		virtual vector<vertex> FindCluster(vertex u)
			{ return manager.FindCluster(u); }
		virtual int CountClusters()
			{ return manager.CountClusters(); }
		virtual vector<vertex> GetCluster(int index)
		{ return manager.GetCluster(index); }

		void ParseArguments(const vector<string>& arguments) override;
};

//------------------------------------------------------------------------------

#endif // ALGORITHM_H
