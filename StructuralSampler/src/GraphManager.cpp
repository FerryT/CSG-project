#include <map>
#include <vector>

#include "GraphManager.h"

//------------------------------------------------------------------------------

template <typename T> class Cluster;
typedef std::map<vertex, Cluster<vertex> *> ClusterPool;
typedef std::vector<Cluster<vertex> *> ClusterIndex;

//==============================================================================

template <typename T>
class Cluster
{
	public:
		Cluster(T val) : value(val), parent(this),
			next(NULL), last(this), count(1) {}
		~Cluster()
			{ crumble(); }
		void operator +=(Cluster &other)
			{ merge(other); }
		operator size_t()
			{ return find()->count; }
		operator std::vector<T>()
			{ return fetch(); }
		bool IsRoot() const
			{ return parent == this; }
		const Cluster &GetRoot()
			{ return *find(); }
	private:
		T value;
		Cluster<T> *parent;
		Cluster<T> *next;
		Cluster<T> *last;
		size_t count;

		Cluster<T> *find()
		{
			if (parent != this)
				parent = parent->find();
			return parent;
		}

		void merge(Cluster &other)
		{
			Cluster<T> *a = find();
			Cluster<T> *b = other.find();
			if (a != b)
			{
				a->parent = b;
				b->last->next = a;
				b->last = a->last;
				b->count += a->count;
			}
		}
		
		void crumble()
		{
			for (Cluster<T> *c = find(), *next; c != NULL; c = next)
			{
				next = c->next;
				c->parent = c;
				c->next = NULL;
				c->last = c;
				c->count = 1;
			}
		}
		
		std::vector<T> fetch()
		{
			Cluster<T> *c = find();
			std::vector<T> vs;
			vs.reserve(c->count);
			for (; c != NULL; c = c->next)
				vs.push_back(c->value);
			return vs;
		}
};

//==============================================================================

struct GraphManager::Data
{
	public:
	Data() : Index(index), index_cached(true) {}
	~Data();
	
	void AddVertex(const vertex &v);
	void RemoveVertex(const vertex &v);
	bool HasVertex(const vertex &v) const;
	Cluster<vertex> &GetCluster(const vertex &v);
	
	void Merge(const Edge &e);
	
	void AddEdge(const Edge &e);
	void RemoveEdge(const Edge &e);
	Edges FindEdges(const vertex &v) const;
	bool HasEdgesWith(const vertex &v) const;
	
	const ClusterIndex &Index;
	void InvalidateIndex();
	void UpdateIndex();
	
	private:
	ClusterPool pool;
	ClusterIndex index;
	bool index_cached;
	Graph graph;
	Less2Graph l2graph;
};

//------------------------------------------------------------------------------

GraphManager::Data::~Data()
{
	for (ClusterPool::iterator it = pool.begin(); it != pool.end(); ++it)
		delete it->second;
}

//------------------------------------------------------------------------------

void GraphManager::Data::AddVertex(const vertex &v)
{
	pool[v] = new Cluster<vertex>(v);
}

//------------------------------------------------------------------------------

void GraphManager::Data::RemoveVertex(const vertex &u)
{
	// Find vertices belonging to same cluster
	Vertices vs = *(pool[u]);
	
	// Remove vertex from clustering
	delete pool[u];
	pool.erase(u);
	
	// Recluster
	for (vertex v : vs)
		if (u != v)
			for (const Edge &e : FindEdges(v))
				Merge(e);
}

//------------------------------------------------------------------------------

bool GraphManager::Data::HasVertex(const vertex &v) const
{
	return pool.count(v) != 0;
}

//------------------------------------------------------------------------------

Cluster<vertex> &GraphManager::Data::GetCluster(const vertex &v)
{
	return *pool[v];
}

//------------------------------------------------------------------------------

void GraphManager::Data::Merge(const Edge &e)
{
	(*pool[e.v1]) += (*pool[e.v2]);
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
		begin = graph.upper_bound(Edge(v, v));
		end = graph.lower_bound(Edge(v + 1, v + 1));
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
	if (graph.upper_bound(Edge(v, v)) != graph.lower_bound(Edge(v + 1, v + 1)))
		return true;
	
	if (l2graph.lower_bound(Edge(0, v)) != l2graph.lower_bound(Edge(0, v + 1)))
		return true;
	
	return false;
}

//------------------------------------------------------------------------------

void GraphManager::Data::InvalidateIndex()
{
	index_cached = false;
}

//------------------------------------------------------------------------------

void GraphManager::Data::UpdateIndex()
{
	if (index_cached) return;
	
	index.clear();
	
	ClusterPool::const_iterator it;
	for (it = pool.begin(); it != pool.end(); ++it)
		if (it->second->IsRoot())
			index.push_back(it->second);
	
	index_cached = true;
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
	data->InvalidateIndex();
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
	
	data->InvalidateIndex();
}

//------------------------------------------------------------------------------

bool GraphManager::ConstraintSatisfied()
{
	data->UpdateIndex();
	for (Cluster<vertex> *cluster : data->Index)
		if (((size_t) *cluster) > maxClusterSize)
			return false;
	return true;
}

//------------------------------------------------------------------------------

clusterid GraphManager::FindClusterIndex(vertex u)
{
	if (!data->HasVertex(u))
		return unclustered;
	
	data->UpdateIndex();
	clusterid id = 0;
	const Cluster<vertex> *root = &data->GetCluster(u).GetRoot();
	for (Cluster<vertex> *cluster : data->Index)
	{
		if (cluster != root)
			++id;
		else
			return id;
	}
			
	return unclustered;
}

//------------------------------------------------------------------------------

Vertices GraphManager::FindCluster(vertex u)
{
	if (!data->HasVertex(u))
		return Vertices();
	
	return data->GetCluster(u);
}

//------------------------------------------------------------------------------

clusterid GraphManager::CountClusters()
{
	data->UpdateIndex();
	return data->Index.size();
}

//------------------------------------------------------------------------------

Vertices GraphManager::GetCluster(clusterid id)
{
	data->UpdateIndex();
	if (id >= data->Index.size())
		throw "[GraphManager::GetCluster] index out of bounds!";
	
	return *data->Index[id];
}

//------------------------------------------------------------------------------
