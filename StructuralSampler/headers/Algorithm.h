#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "GraphInput.h"
#include "Graph.h"
#include "GraphManager.h"
#include "ReservoirManager.h"

//------------------------------------------------------------------------------

class Algorithm: public Output {
	public:
		virtual void Add(Edge e) = 0;
		virtual void Remove(Edge e) = 0;

		// The query methods:
		// Finds the cluster index
		virtual clusterid FindClusterIndex(vertex u) = 0;
		// finds all the verices in the cluster of u
		virtual Vertices FindCluster(vertex u) = 0;
		// returns the number of clusters
		virtual clusterid CountClusters() = 0;
		// gets all vertices of cluster
		virtual Vertices GetCluster(clusterid index) = 0;

		//parses arguments for configuration
		virtual void ParseArguments(const Strings &arguments);
};

//------------------------------------------------------------------------------

class Metis : public Algorithm {
	public:
		Metis(clusterid maxClusterSize);
		virtual ~Metis();

		virtual void SetMaxClusterSize(clusterid maxClusterSize);

		virtual void Add(Edge e);
		virtual void Remove(Edge e);

		void ParseArguments(const Strings& arguments);

		virtual clusterid FindClusterIndex(vertex u);
		virtual Vertices FindCluster(vertex u);
		virtual clusterid CountClusters();
		virtual Vertices GetCluster(clusterid index);
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
		StructuralSampler(clusterid maxClusterSize = 10);
		
		virtual void Add(Edge e);
		virtual void Remove(Edge e);
		
		virtual clusterid FindClusterIndex(vertex u)
			{ return manager.FindClusterIndex(u); }
		virtual Vertices FindCluster(vertex u)
			{ return manager.FindCluster(u); }
		virtual clusterid CountClusters()
			{ return manager.CountClusters(); }
		virtual Vertices GetCluster(clusterid index)
		{ return manager.GetCluster(index); }

		void ParseArguments(const Strings &arguments);
};

//------------------------------------------------------------------------------

#endif // ALGORITHM_H
