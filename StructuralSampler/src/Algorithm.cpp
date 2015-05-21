#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <algorithm>

#include "metis.h"

#include "Algorithm.h"

//------------------------------------------------------------------------------

struct Metis::Data
{
	idx_t num_nodes;
	idx_t num_edges;
	idx_t num_clusters;

	idx_t *node_indices;
	idx_t *edges;
	idx_t *clustering;

	Data(int numClusters)
		: num_nodes(0), num_edges(0), num_clusters(numClusters),
		node_indices(NULL), edges(NULL), clustering(NULL) {}
	const Data &operator =(const Data &other)
	{
		if (&other != this)
			memcpy(this, &other, sizeof (Data));
		return *this;
	}
};

//------------------------------------------------------------------------------

Metis::Metis(int numClusters)
{
	data = new Data(numClusters);
}

//------------------------------------------------------------------------------

Metis::~Metis()
{
	if (data)
		delete data;
}

//------------------------------------------------------------------------------

void Metis::Add(Edge e)
{
	// Prepare next graph memory footprint
	Data next = *data;
	next.num_nodes = std::max((vertex) data->num_nodes, std::max(e.v1, e.v2));
	next.num_edges = data->num_edges + 2;

	next.edges = new idx_t[next.num_edges];
	if (!data->num_nodes || (next.num_nodes != data->num_nodes))
	{
		next.node_indices = new idx_t[next.num_nodes];
		next.clustering = new idx_t[next.num_nodes];
	}
	
	// Copy graph
	idx_t v1, v2;
	if (e.v1 == e.v2)
		throw "Error while adding edges using the Metis algorithm: self-loop detected (which is not supported).";
	else if (e.v1 < e.v2)
	{
		v1 = e.v1;
		v2 = e.v2;
	}
	else
	{
		v1 = e.v2;
		v2 = e.v1;
	}
	
	// Todo: node indices, edges
	// Todo: call metis

	// Clean up previous graph memory footprint
	if (data->node_indices && (next.node_indices != data->node_indices))
		delete[] data->node_indices;
	if (data->clustering && (next.clustering != data->clustering))
		delete[] data->clustering;
	if (data->edges)
		delete[] data->edges;
	*data = next;
}

//------------------------------------------------------------------------------

void Metis::Remove(Edge e)
{
}

//------------------------------------------------------------------------------

int Metis::FindClusterIndex(vertex u)
{
	return (int) data->clustering[u];
}

//------------------------------------------------------------------------------

vector<vertex> Metis::FindCluster(vertex u)
{
	idx_t cluster = data->clustering[u];
	
	vector<vertex> vs;
	for (idx_t v = data->num_nodes - 1; v >= 0; --v)
		if (data->clustering[v] == cluster)
			vs.push_back(v);
	
	return vs;
}

//------------------------------------------------------------------------------

int Metis::CountClusters()
{
	return data->num_clusters;
}

//==============================================================================

StructuralSampler::StructuralSampler(int maxClusterSize) : manager(maxClusterSize)
{
	srand(time(NULL));
}

//------------------------------------------------------------------------------

void StructuralSampler::Add(Edge newEdge)
{
	//TODO: sampling method, mentioned under C. 3)
	double pos = (double) rand() / (double) RAND_MAX;
	newEdge.p = pos;
	strReservoir.Add(newEdge);
	manager.Add(newEdge);
	if (!manager.ConstraintSatisfied())
	{
		Edge currEdge;
		while (!manager.ConstraintSatisfied())
		{
			currEdge = strReservoir.RemoveLast();
			pos = currEdge.p;
			supReservoir.Add(currEdge);
			manager.RemoveExact(currEdge);
		}
		vector<Edge> searchResults = supReservoir.GetEdges(pos);
		for (vector<Edge>::size_type i = 0; i < searchResults.size(); i++)
		{
			currEdge = searchResults[i];
			manager.Add(currEdge);
			if (!manager.ConstraintSatisfied())
			{
				manager.RemoveExact(currEdge);
			}
			else
			{
				supReservoir.RemoveExact(currEdge);
				strReservoir.Add(currEdge);
			}
		}
	}
}

//------------------------------------------------------------------------------

void StructuralSampler::Remove(Edge theEdge)
{
	bool supSucces = supReservoir.Remove(theEdge);
	if (strReservoir.HasEdge(theEdge))
	{
		double rmEdgeP = strReservoir.Remove(theEdge);
		manager.Remove(theEdge);
		vector<Edge> searchResults = supReservoir.GetEdges(rmEdgeP);
		Edge currEdge;
		for (vector<Edge>::size_type i = 0; i < searchResults.size(); i++)
		{
			currEdge = searchResults[i];
			manager.Add(currEdge);
			if (!manager.ConstraintSatisfied())
			{
				manager.RemoveExact(currEdge);
			}
			else
			{
				supReservoir.RemoveExact(currEdge);
				strReservoir.Add(currEdge);
			}
		}
	}
}

//------------------------------------------------------------------------------
