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
		void operator --()
			{ crumble(); }
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
	Data() : MaxCount(max_count), Index(index), index_cached(true), max_count(0) {}
	~Data();
	
	void AddVertex(const vertex &v);
	void RemoveVertex(const vertex &v);
	bool HasVertex(const vertex &v) const;
	Cluster<vertex> &GetCluster(const vertex &v);
	
	void Merge(const Edge &e);
	const size_t &MaxCount;
	
	void AddEdge(const Edge &e);
	void RemoveEdge(const Edge &e);
	Edges FindEdges(const vertex &v) const;
	bool HasEdgesWith(const vertex &v) const;
	
	const ClusterIndex &Index;
	void InvalidateIndex();
	void UpdateIndex();
	
	#ifdef GMTEST
	void Assert();
	#endif
	
	private:
	ClusterPool pool;
	ClusterIndex index;
	bool index_cached;
	Graph graph;
	Less2Graph l2graph;
	size_t max_count;
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
	max_count = std::max(max_count, (size_t) *pool[e.v1]);
}

//------------------------------------------------------------------------------

void GraphManager::Data::RemoveEdge(const Edge &e)
{
	// We assume that existing edges always are in the same cluster
	
	// Whether the deleted edge may decrease the max count
	bool max_cluster = (size_t) *pool[e.v1] >= max_count;
	
	// Remove edge from graph
	graph.erase(e);
	l2graph.erase(e);
	
	// Find vertices belonging to same cluster
	Vertices vs = *pool[e.v1];
	
	// Crumble the cluster
	--*pool[e.v1];
	
	// Recluster
	for (vertex v : vs)
		for (const Edge &e : FindEdges(v))
			Merge(e);
	
	// Recalculate count
	if (max_cluster)
	{
		max_count = 0;
		for (ClusterPool::iterator it = pool.begin(); it != pool.end(); ++it)
			max_count = std::max(max_count, (size_t) *it->second);
	}
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

//------------------------------------------------------------------------------

#ifdef GMTEST

typedef std::set<vertex> DebugList;
typedef std::vector<DebugList> DebugCluster;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

inline clusterid debug_find(const DebugCluster &cloud, vertex v)
{
	for (size_t i = 0; i < cloud.size(); ++i)
		if (cloud[i].count(v))
			return i;
	return unclustered;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

inline void debug_throw(const DebugCluster &cloud, const char *msg)
{
	printf("CC: %d\n", cloud.size());
	for (const DebugList &vs : cloud)
	{
		for (vertex v : vs)
			printf("%lu ", v);
		putchar(10);
	}
	throw msg;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void GraphManager::Data::Assert()
{
	DebugCluster cloud;
	for (const Edge &e : graph)
	{
		clusterid c1 = debug_find(cloud, e.v1);
		clusterid c2 = debug_find(cloud, e.v2);
		if (c1 == unclustered && c2 == unclustered)
		{
			vertex vs[] = {e.v1, e.v2};
			cloud.push_back(DebugList(vs, vs + 2));
		}
		else if (c1 == unclustered)
			cloud[c2].insert(e.v1);
		else if (c2 == unclustered)
			cloud[c1].insert(e.v2);
		else if (c1 != c2)
		{
			cloud[c1].insert(cloud[c2].begin(), cloud[c2].end());
			cloud.erase(cloud.begin() + c2);
		}
	}
	
	DebugList inpool;
	std::set<Cluster<vertex> *> clusters;
	for (const DebugList &vs : cloud)
	{
		Cluster<vertex> *c = (Cluster<vertex> *) &pool[*vs.begin()]->GetRoot();
		if (clusters.count(c) > 0)
			debug_throw(cloud, "GM assertion failed: Clusters merged");
		
		clusters.insert(c);
		for (vertex v : vs)
		{
			inpool.insert(v);
			if (!pool.count(v))
				debug_throw(cloud, "GM assertion failed: Vertex missing");
			
			if ((c != NULL) && (c != &pool[v]->GetRoot()))
				debug_throw(cloud, "GM assertion failed: Clustering invalid");
		}
		if (vs.size() != *c)
			debug_throw(cloud, "GM assertion failed: Cluster size invalid");
	}
	
	ClusterPool::iterator it;
	for (it = pool.begin(); it != pool.end(); ++it)
		if (!inpool.count(it->first))
			debug_throw(cloud, "GM assertion failed: Vertex superfluous");
	
	
}

#endif /* GMTEST */

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
	
	#ifdef GMTEST
	try { data->Assert(); } catch (const char *msg)
	{
		printf("Inconsistency after Add (%lu,%lu)\n", e.v1, e.v2);
		throw msg;
	}
	#endif
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
	
	#ifdef GMTEST
	try { data->Assert(); } catch (const char *msg)
	{
		printf("Inconsistency after Remove (%lu,%lu)\n", e.v1, e.v2);
		throw msg;
	}
	#endif
}

//------------------------------------------------------------------------------

bool GraphManager::ConstraintSatisfied()
{
	return data->MaxCount <= maxClusterSize;
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
