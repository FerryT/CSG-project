#include <map>

#include "GraphManager.h"

//------------------------------------------------------------------------------

class Cluster;
typedef std::map<vertex, Cluster *> ClusterPool;
typedef std::map<Cluster *, clusterid> ClusterIDPool;

//==============================================================================

class Cluster
{
	public:
		Cluster(ClusterIDPool *ids) : parent(this), pool(ids), count(1)
		{
			(*pool)[this] = pool->size();
		}
		~Cluster()
		{
			if (parent == this)
			{
				pool->erase(this);
				reindex();
			}
		}
		Cluster *operator +=(Cluster *other)
		{
			merge(other);
			return this;
		}
		operator clusterid()
		{
			return (*pool)[find()];
		}
		clusterid Count()
		{
			return find()->count;
		}
		bool IsRoot() const
		{
			return parent == this;
		}
		void Reset()
		{
			count = 1;
			if (parent != this)
			{
				(*pool)[this] = pool->size();
				parent = this;
			}
		}
	private:
		Cluster *parent;
		ClusterIDPool *pool;
		clusterid count;

		Cluster *find()
		{
			if (parent != this)
				parent = parent->find();
			return parent;
		}

		void merge(Cluster *other)
		{
			Cluster *a = find();
			Cluster *b = other->find();
			if (a != b)
			{
				a->parent = b;
				b->count += a->count;
				pool->erase(a);
				reindex();
			}
		}

		void reindex()
		{
			clusterid id = 0;
			ClusterIDPool::iterator it;
			for (it = pool->begin(); it != pool->end(); ++it)
				it->second = id++;
		}
};

//==============================================================================

struct GraphManager::Data
{
	ClusterPool pool;
	ClusterIDPool ids;
	Graph graph;
	Less2Graph l2graph;

	void AddVertex(const vertex &v);
	void RemoveVertex(const vertex &v);
	bool HasVertex(const vertex &v) const;
	
	void Merge(const Edge &e);
	
	void AddEdge(const Edge &e);
	void RemoveEdge(const Edge &e);
	Edges FindEdges(const vertex &v) const;
	bool HasEdgesWith(const vertex &v) const;
};

//------------------------------------------------------------------------------

void GraphManager::Data::AddVertex(const vertex &v)
{
	pool[v] = new Cluster(&ids);
}

//------------------------------------------------------------------------------

void GraphManager::Data::RemoveVertex(const vertex &v)
{
	Vertices vs; // Holds vertices belonging to resolved cluster
	std::vector<Cluster *> cs; // Holds cluster objects being resolved
	
	Cluster *c = pool[v];
	clusterid cid = *c;
	vs.reserve(c->Count() - 1);
	cs.reserve(c->Count() - 1);
	pool.erase(v); // Remove vertex
	
	// Find vertices and cluster objects belonging to teh cluster
	for (ClusterPool::iterator it = pool.begin(); it != pool.end(); ++it)
	{
		if (*it->second == cid)
		{
			vs.push_back(it->first);
			cs.push_back(it->second);
		}
	}

	// Break up cluster
	for (Cluster *cluster : cs)
		cluster->Reset();
	
	// Free removed cluster: now since it could have been a cluster root
	delete c;
	
	// Recluster
	for (const vertex &v : vs)
		for (const Edge &e : FindEdges(v))
			Merge(e);
}

//------------------------------------------------------------------------------

bool GraphManager::Data::HasVertex(const vertex &v) const
{
	return (pool.count(v) == 1);
}

//------------------------------------------------------------------------------

void GraphManager::Data::Merge(const Edge &e)
{
	(*pool[e.v1]) += pool[e.v2];
}

//------------------------------------------------------------------------------

void GraphManager::Data::AddEdge(const Edge &e)
{
	graph.insert(e);
	l2graph.insert(e);
}

//------------------------------------------------------------------------------

void GraphManager::Data::RemoveEdge(const Edge &e)
{
	graph.erase(e);
	l2graph.erase(e);
}

//------------------------------------------------------------------------------

Edges GraphManager::Data::FindEdges(const vertex &v) const
{
	Edges es;
	
	{
		Graph::iterator begin, it, end;
		begin = graph.lower_bound(Edge(v, 0));
		end = graph.lower_bound(Edge(v + 1, 0));
		for (it = begin; it != end; ++it)
			es.push_back(*it);
	}
	
	{
		Less2Graph::iterator begin, it, end;
		begin = l2graph.lower_bound(Edge(0, v));
		end = l2graph.lower_bound(Edge(0, v + 1));
		for (it = begin; it != end; ++it)
			es.push_back(*it);
	}
	
	return es;
}

//------------------------------------------------------------------------------

bool GraphManager::Data::HasEdgesWith(const vertex &v) const
{
	if (graph.lower_bound(Edge(v, 0)) != graph.lower_bound(Edge(v + 1, 0)))
		return true;
	
	if (l2graph.lower_bound(Edge(0, v)) != l2graph.lower_bound(Edge(0, v + 1)))
		return true;
	
	return false;
}

//==============================================================================

GraphManager::GraphManager(clusterid max) : maxClusterSize(max)
{
	data = new Data;
	if (!data)
		throw "Graphmanager: memory issue.";
}

//------------------------------------------------------------------------------

GraphManager::~GraphManager()
{
	delete data;
	data->ids.clear();
	
	ClusterPool::iterator it;
	for (it = data->pool.begin(); it != data->pool.end(); ++it)
		delete it->second;
	data->pool.clear();
}

//------------------------------------------------------------------------------

void GraphManager::Add(const Edge &e)
{
	// Adds an edge to the graph
	// When an edge contains non-existing verices, add them to the pool
	// Merge whatever clusters are joined by this edge
	// (When the clusters are already joined nothing will happen)
	
	if (!data->HasVertex(e.v1))
		data->AddVertex(e.v1);
	
	if (!data->HasVertex(e.v2))
		data->AddVertex(e.v2);
	
	data->Merge(e);
	data->AddEdge(e);
}

//------------------------------------------------------------------------------

void GraphManager::RemoveExact(const Edge &e)
{
	// We assume only one edge can exist between two nodes:
	// The normal remove method is already exact
	Remove(e);
}

//------------------------------------------------------------------------------

void GraphManager::Remove(const Edge &e)
{
	// Removes an edge from the graph
	// When vertices become orphaned by this action, remove them as well
	// Clusters with removed vertices are reclustered by first isolating each
	//   vertex in its own cluster and them merging them with each connected
	//   edge that is still in the graph.
	
	data->RemoveEdge(e);
	
	if (!data->HasEdgesWith(e.v1))
		data->RemoveVertex(e.v1);
	
	if (!data->HasEdgesWith(e.v2))
		data->RemoveVertex(e.v2);
}

//------------------------------------------------------------------------------

bool GraphManager::ConstraintSatisfied()
{
	ClusterIDPool::iterator it;
	for (it = data->ids.begin(); it != data->ids.end(); ++it)
		if (it->first->Count() > maxClusterSize)
			return false;
	return true;
}

//------------------------------------------------------------------------------

clusterid GraphManager::FindClusterIndex(vertex u)
{
	if (!data->pool.count(u))
		throw "[GraphManager::FindClusterIndex] index out of bounds!";
	return (clusterid) *data->pool[u];
}

//------------------------------------------------------------------------------

Vertices GraphManager::FindCluster(vertex u)
{
	clusterid i = FindClusterIndex(u);
	
	Vertices vs;
	ClusterPool::iterator it;
	for (it = data->pool.begin(); it != data->pool.end(); ++it)
		if (*it->second == i)
			vs.push_back(it->first);
	return vs;
}

//------------------------------------------------------------------------------

clusterid GraphManager::CountClusters()
{
	return data->ids.size();
}

//------------------------------------------------------------------------------

Vertices GraphManager::GetCluster(clusterid i)
{
	if (i >= data->ids.size())
		throw "[GraphManager::GetCluster] index out of bounds!";
	
	Vertices vs;
	ClusterPool::iterator it;
	for (it = data->pool.begin(); it != data->pool.end(); ++it)
		if (*it->second == i)
			vs.push_back(it->first);
	return vs;
}

//------------------------------------------------------------------------------
