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
	
	bool cached;

	Data(int numClusters)
		: num_nodes(0), num_edges(0), num_clusters(numClusters),
		node_indices(NULL), edges(NULL), clustering(NULL),
		cached(true) {}
	void UpdateClusters();
};

//------------------------------------------------------------------------------

void Metis::Data::UpdateClusters()
{
	if (cached)
		return;
	
	// Todo: call Metis
	
	cached = true;
}

//------------------------------------------------------------------------------

Metis::Metis(int numClusters)
{
	data = new Data(numClusters);
}

//------------------------------------------------------------------------------

Metis::~Metis()
{
	if (data)
	{
		if (data->node_indices)
			delete[] data->node_indices;
		if (data->edges)
			delete[] data->edges;
		if (data->clustering)
			delete[] data->clustering;
		delete data;
	}
}

//------------------------------------------------------------------------------

void Metis::Add(Edge e)
{
	idx_t v1, v2;
	if (e.v1 == e.v2)
		throw "Error while adding edges using the Metis algorithm:"
			"self-loop detected (which is not supported).";
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
	idx_t nodes_needed = v2 + 1;
	
	if (data->num_nodes < 1)
	{
		// First edge added
		data->num_nodes = nodes_needed;
		data->num_edges = 2;
		data->node_indices = new idx_t[nodes_needed];
		data->edges = new idx_t[2];
		data->clustering = new idx_t[nodes_needed];
		
		idx_t i = 0;
		while (i <= v1)
			data->node_indices[i++] = 0;
		while (i <= v2)
			data->node_indices[i++] = 1;
		
		data->edges[0] = v2;
		data->edges[1] = v1;
	}
	else
	{
		if (data->num_nodes < nodes_needed) // Node range increases
		{
			// Expand node indices
			{
				idx_t *old_indices = data->node_indices;
				data->node_indices = new idx_t[nodes_needed];
				memcpy(data->node_indices, old_indices, sizeof (old_indices));
				delete[] old_indices;
			}
			
			idx_t i = data->num_nodes;
			idx_t j = data->node_indices[i - 1];
			while (i < nodes_needed)
				data->node_indices[i++] = j;
			
			delete[] data->clustering;
			data->clustering = new idx_t[nodes_needed];
			data->num_clusters = nodes_needed;
		}
		{
			// Update edges
			idx_t i = data->node_indices[v1 + 1];
			idx_t j = data->node_indices[v2 + 1];
			idx_t k = data->num_edges;
			
			idx_t *old_edges = data->edges;
			data->edges = new idx_t[data->num_edges];
			
			memcpy(data->edges, old_edges, i * sizeof (idx_t));
			data->edges[i] = v2;
			memcpy(data->edges + i + 1, old_edges + i, (j - i) * sizeof (idx_t));
			data->edges[j] = v1;
			memcpy(data->edges + j + 2, old_edges + j, (k - j) * sizeof (idx_t));
			
			delete[] old_edges;
		}
		{
			// Update node indices
			idx_t i = v1 + 1;
			while (i <= v2)
				data->node_indices[i++] += 1;
			while (i < data->num_nodes)
				data->node_indices[i++] += 2;
		}
		data->num_edges += 2;
	}
	
	data->cached = false;
}

//------------------------------------------------------------------------------

void Metis::Remove(Edge e)
{
	throw "[Metis::Remove] unimplemented!";
}

//------------------------------------------------------------------------------

int Metis::FindClusterIndex(vertex u)
{
	if (u >= data->num_nodes)
		throw "[Metis::FindClusterIndex] index out of bounds!";
	data->UpdateClusters();
	return (int) data->clustering[u];
}

//------------------------------------------------------------------------------

vector<vertex> Metis::FindCluster(vertex u)
{
	if (u >= data->num_nodes)
		throw "[Metis::FindClusterIndex] index out of bounds!";
	data->UpdateClusters();
	
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
