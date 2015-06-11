#include <cstdlib>
#include <ctime>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "metis.h"

#include "Algorithm.h"

//------------------------------------------------------------------------------

void Algorithm::ParseArguments(const Strings &arguments)
{
	std::cout << "arguments for this algorithm are not yet parsed" << std::endl;
}

//==============================================================================

class MetisOptions
{
	private:
	idx_t options[METIS_NOPTIONS];
	
	public:
	MetisOptions()
	{
		METIS_SetDefaultOptions(options);
		options[METIS_OPTION_OBJTYPE] = METIS_OBJTYPE_CUT;
		options[METIS_OPTION_CTYPE]   = METIS_CTYPE_SHEM;
		options[METIS_OPTION_IPTYPE]  = METIS_IPTYPE_METISRB;
		options[METIS_OPTION_RTYPE]   = METIS_RTYPE_GREEDY;
		options[METIS_OPTION_NO2HOP]  = 0;
		options[METIS_OPTION_MINCONN] = 0;
		options[METIS_OPTION_CONTIG]  = 0;
		options[METIS_OPTION_SEED]    = -1;
		options[METIS_OPTION_NITER]   = 10;
		options[METIS_OPTION_NCUTS]   = 1;
		options[METIS_OPTION_UFACTOR] = -1;
		options[METIS_OPTION_DBGLVL]  = 0;
	}
	
	operator idx_t *() { return options; }
};

//------------------------------------------------------------------------------

struct Metis::Data
{
	idx_t num_nodes;
	idx_t num_edges;
	idx_t max_cluster_size;

	idx_t *node_indices;
	idx_t *edges;
	idx_t *clustering;
	
	bool cached;
	MetisOptions options;
	
	std::unordered_map<vertex, idx_t> alias;
	std::vector<vertex> dealias;

	Data(int maxClusterSize)
		: num_nodes(0), num_edges(0), max_cluster_size(maxClusterSize),
		node_indices(NULL), edges(NULL), clustering(NULL),
		cached(true), options() {}
	void UpdateClusters();
	void Debug();
};

//------------------------------------------------------------------------------

void Metis::Data::UpdateClusters()
{
	if (cached)
		return;
	
	/** /Debug();//*/
	
	idx_t ncon = 1;
	//approximation of the parts from max cluster size
	idx_t nparts = ceil(static_cast<double>(num_nodes)/max_cluster_size);
	idx_t objval;
	if (nparts == 1)
		nparts = 2;
	
	int status = METIS_PartGraphKway(&num_nodes, &ncon,
		node_indices,	edges,
		NULL, NULL, NULL,
		&nparts, NULL, NULL, options,
		&objval, clustering); // output
	
	if (status == METIS_ERROR_INPUT)
		throw "Metis error: invalid input or option.";
	if (status == METIS_ERROR_MEMORY)
		throw "Metis error: issues with memory.";
	if (status == METIS_ERROR)
		throw "Metis error: unable to calculate clustering.";
	
	cached = true;
}

//------------------------------------------------------------------------------

void Metis::Data::Debug()
{
	FILE *fp = fopen("debug.txt", "wt");
	fprintf(fp, "%lu %lu", num_nodes, num_edges / 2);
	for (idx_t i = 0; i < num_nodes; ++i)
	{
		fputs("\n", fp);
		for (idx_t j = node_indices[i]; j < node_indices[i+1]; ++j)
			fprintf(fp, "%lu ", edges[j] + 1);
	}
	fclose(fp);
}

//------------------------------------------------------------------------------

Metis::Metis(clusterid maxClusterSize)
{
	if (maxClusterSize < 2)
		throw "Metis error: the number of clusters must be greater than 1.";
	
	data = new Data(maxClusterSize);
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

void Metis::SetMaxClusterSize(clusterid maxClusterSize)
{
	if (data)
		data->max_cluster_size = maxClusterSize;
}

//------------------------------------------------------------------------------

void Metis::Add(Edge e)
{
	idx_t v1, v2;
	if (e.v1 == e.v2)
		throw "Metis error: self-loop detected while adding edges"
			"(which is not supported).";
	
	if (!data->alias.count(e.v1))
	{
		v1 = data->dealias.size();
		data->alias[e.v1] = v1;
		data->dealias.push_back(e.v1);
	}
	else
		v1 = data->alias[e.v1];
	
	if (!data->alias.count(e.v2))
	{
		v2 = data->dealias.size();
		data->alias[e.v2] = v2;
		data->dealias.push_back(e.v2);
	}
	else
		v2 = data->alias[e.v2];
	
	if (v1 > v2)
		std::swap(v1,v2);
	
	idx_t nodes_needed = v2 + 1;
	
	if (data->num_nodes < 1)
	{
		// First edge added
		data->num_nodes = nodes_needed;
		data->num_edges = 2;
		data->node_indices = new idx_t[nodes_needed + 1];
		data->edges = new idx_t[2];
		data->clustering = new idx_t[nodes_needed];
		
		idx_t i = 0;
		while (i <= v1)
			data->node_indices[i++] = 0;
		while (i <= v2)
			data->node_indices[i++] = 1;
		data->node_indices[i] = 2;
		
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
				data->node_indices = new idx_t[nodes_needed + 1];
				memcpy(data->node_indices, old_indices,
					(data->num_nodes + 1) * sizeof (idx_t));
				delete[] old_indices;
			}
			
			idx_t i = data->num_nodes;
			idx_t j = data->node_indices[i];
			while (i <= nodes_needed)
				data->node_indices[i++] = j;
			
			delete[] data->clustering;
			data->clustering = new idx_t[nodes_needed];
			data->num_nodes = nodes_needed;
		}
		{
			// Update edges
			idx_t i = data->node_indices[v1 + 1];
			idx_t j = data->node_indices[v2 + 1];
			idx_t k = data->num_edges;
			
			data->num_edges += 2;
			idx_t *old_edges = data->edges;
			data->edges = new idx_t[data->num_edges];
			
			memcpy(data->edges, old_edges, i * sizeof (idx_t));
			data->edges[i] = v2;
			memcpy(data->edges + i + 1, old_edges + i, (j - i) * sizeof (idx_t));
			data->edges[j + 1] = v1;
			memcpy(data->edges + j + 2, old_edges + j, (k - j) * sizeof (idx_t));
			
			delete[] old_edges;
		}
		{
			// Update node indices
			idx_t i = v1 + 1;
			while (i <= v2)
				data->node_indices[i++] += 1;
			while (i <= data->num_nodes)
				data->node_indices[i++] += 2;
		}
	}
	
	data->cached = false;
}

//------------------------------------------------------------------------------

void Metis::Remove(Edge e)
{
	idx_t v1, v2;
	if (e.v1 == e.v2)
		throw "Metis error: self-loop detected while removing edges"
			"(which is not supported).";
	
	if (!data->alias.count(e.v1))
	{
		v1 = data->dealias.size();
		data->alias[e.v1] = v1;
		data->dealias.push_back(e.v1);
	}
	else
		v1 = data->alias[e.v1];
	
	if (!data->alias.count(e.v2))
	{
		v2 = data->dealias.size();
		data->alias[e.v2] = v2;
		data->dealias.push_back(e.v2);
	}
	else
		v2 = data->alias[e.v2];
	
	if (v1 > v2)
		std::swap(v1,v2);
	
	if (v1 >= data->num_nodes || v2 >= data->num_nodes)
		throw "Metis error: tried to remove an unexisting edge.";
	
	idx_t i = data->node_indices[v1];
	while (data->edges[i] != v2)
		if (i++ >= data->node_indices[v1 + 1])
			throw "Metis error: tried to remove an unexisting edge.";
	
	idx_t j = data->node_indices[v2];
	while (data->edges[j] != v1)
		if (j++ >= data->node_indices[v2 + 1])
			throw "Metis error: tried to remove an unexisting edge.";
	
	if (data->num_nodes == 1)
	{
		// Last edge removed
		data->num_nodes = 0;
		data->num_edges = 0;
		delete[] data->node_indices;
		delete[] data->edges;
		delete[] data->clustering;
		data->node_indices = NULL;
		data->edges = NULL;
		data->clustering = NULL;
	}
	else
	{
		{
			// Update edges
			idx_t k = data->num_edges;
			
			data->num_edges -= 2;
			idx_t *old_edges = data->edges;
			data->edges = new idx_t[data->num_edges];
			
			memcpy(data->edges, old_edges, i * sizeof (idx_t));
			memcpy(data->edges + i, old_edges + i + 1, (j - i - 1) * sizeof (idx_t));
			memcpy(data->edges + j - 1, old_edges + j + 1, (k - j - 1) * sizeof (idx_t));
			
			delete[] old_edges;
		}
		{
			// Update node indices
			idx_t i = v1 + 1;
			while (i <= v2)
				data->node_indices[i++] -= 1;
			while (i <= data->num_nodes)
				data->node_indices[i++] -= 2;
		}
	}
	
	data->cached = false;
}

//------------------------------------------------------------------------------

void Metis::ParseArguments(const Strings &arguments)
{
	if (arguments.size() == 0)
	{
		return;
	}
	else if (arguments.size() == 1)
	{
		this->SetMaxClusterSize(atoi(arguments[0].c_str()));
	}
	else
	{
		throw "Can't parse the parameters for Metis the only argument is num clusters";
	}
}

//------------------------------------------------------------------------------

clusterid Metis::FindClusterIndex(vertex u)
{
	if (!data->alias.count(u))
		throw "[Metis::FindClusterIndex] index out of bounds!";
	data->UpdateClusters();
	return (int) data->clustering[data->alias[u]];
}

//------------------------------------------------------------------------------

Vertices Metis::FindCluster(vertex u)
{
	if (!data->alias.count(u))
		throw "[Metis::FindClusterIndex] index out of bounds!";
	data->UpdateClusters();
	
	idx_t cluster = data->clustering[data->alias[u]];
	
	Vertices vs;
	for (idx_t v = data->num_nodes - 1; v >= 0; --v)
		if (data->clustering[v] == cluster)
			vs.push_back(data->dealias[v]);
	
	return vs;
}

//------------------------------------------------------------------------------

clusterid Metis::CountClusters()
{
	long clusters = ceil(static_cast<double>(data->num_nodes) / data->max_cluster_size);
	if (clusters == 1)
	{
		return 2;
	}
	else
	{
		return clusters;
	}
}

//------------------------------------------------------------------------------

Vertices Metis::GetCluster(clusterid index)
{
	data->UpdateClusters();
	
	Vertices vs;
	for (idx_t v = data->num_nodes - 1; v >= 0; --v)
		if (data->clustering[v] == index)
			vs.push_back(data->dealias[v]);
	
	return vs;
}

//==============================================================================

StructuralSampler::StructuralSampler(clusterid maxClusterSize) : manager(maxClusterSize)
{
}

//------------------------------------------------------------------------------

void StructuralSampler::Add(Edge newEdge)
{
	strReservoir.Add(newEdge);
	manager.Add(newEdge);
	if (!manager.ConstraintSatisfied())
	{
		double pos;
		while (!manager.ConstraintSatisfied())
		{
			const Edge &currEdge = strReservoir.RemoveLast();
			pos = currEdge.p;
			supReservoir.Add(currEdge);
			manager.RemoveExact(currEdge);
		}
		const Edges &searchResults = supReservoir.GetEdges(pos);
		for (Edges::const_iterator it = searchResults.begin(); it != searchResults.end(); ++it)
		{
			const Edge &currEdge = *it;
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
		const Edges &searchResults = supReservoir.GetEdges(rmEdgeP);
		for (Edges::const_iterator it = searchResults.begin(); it != searchResults.end(); ++it)
		{
			const Edge &currEdge = *it;
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

void StructuralSampler::ParseArguments(const Strings &arguments)
{
	if (arguments.size() == 0)
	{
		return;
	}
	else if (arguments.size() == 1)
	{
		this->manager.maxClusterSize = atoi(arguments[0].c_str());
	}
	else
	{
		throw "Can't parse the parameters for StructuralSampler the only argument is the max cluster size";
	}
}

//------------------------------------------------------------------------------
