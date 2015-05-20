#include <stdlib.h>
#include <time.h>
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
	Data(const Data &other) :
		num_nodes(other.num_nodes),
		num_edges(other.num_edges),
		num_clusters(other.num_clusters),
		node_indices(other.node_indices),
		edges(other.edges),
		clustering(other.clustering) {}
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
	Data next = *data;

	next.num_nodes = std::max((vertex) data->num_nodes, std::max(e.v1, e.v2));
	next.num_edges = data->num_edges + 2;
	idx_t *new_node_indices = data->node_indices;
	idx_t *new_edges = data->edges;
	idx_t *new_clustering = data->clustering;

	new_edges = new idx_t[next.num_edges];
	if (!data->num_nodes || (next.num_nodes != data->num_nodes))
	{
		new_node_indices = new idx_t[next.num_nodes];
		new_clustering = new idx_t[next.num_nodes];
	}

	
}

//------------------------------------------------------------------------------

void Metis::Remove(Edge e)
{
}

//------------------------------------------------------------------------------

int Metis::FindClusterIndex(vertex u)
{
}

//------------------------------------------------------------------------------

vector<vertex> Metis::FindCluster(vertex u)
{
}

//------------------------------------------------------------------------------

int Metis::CountClusters()
{
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
