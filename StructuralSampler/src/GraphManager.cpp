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
		Cluster(ClusterIDPool *ids, Cluster * p) : pool(ids), count(1)
		{
			(*pool)[this] = pool->size();
			parent = p;
		}
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
		bool IsRoot()
		{
			return parent == this;
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

	void AddCluster(const vertex &v);
	Vertices RemoveCluster(const vertex &v);
	bool InCluster(const vertex &v) const;
	void MergeCluster(const vertex &v1, const vertex &v2);
	
	Graph graph;
	Less2Graph l2graph;
	
	void AddEdge(const Edge &e);
	void RemoveEdge(const Edge &e);
	Edges FindEdges(const vertex &v);
};

//------------------------------------------------------------------------------

void GraphManager::Data::AddEdge(const Edge &e)
{
	graph.insert(e);
	l2graph.insert(e);
	bool v1New = (pool.count(e.v1) == 0);
	bool v2New = (pool.count(e.v2) == 0);

	if (v1New && v2New)
	{
		Cluster c1(&ids);
		Cluster c2(&ids,&c1);
		pool.insert(std::pair<vertex, Cluster *>(e.v1, &c1));
		pool.insert(std::pair<vertex, Cluster *>(e.v2, &c2));
	}
	else if (v1New)
	{
		Cluster newC(&ids, pool[e.v2]);
		pool.insert(std::pair<vertex, Cluster *>(e.v1, &newC));
	}
	else if (v2New)
	{
		Cluster newC(&ids, pool[e.v1]);
		pool.insert(std::pair<vertex, Cluster *>(e.v2, &newC));
	}
}

//------------------------------------------------------------------------------

void GraphManager::Data::RemoveEdge(const Edge &e)
{
	graph.erase(e);
	l2graph.erase(e);


}

//------------------------------------------------------------------------------

Edges GraphManager::Data::FindEdges(const vertex &v)
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

void GraphManager::Data::AddCluster(const vertex &v)
{
	pool[v] = new Cluster(&ids);
}

//------------------------------------------------------------------------------

Vertices GraphManager::Data::RemoveCluster(const vertex &v)
{
	Vertices vs;
	Cluster *c = pool[v];
	clusterid cid = (clusterid) *c;
	vs.reserve(c->Count());
	
	Cluster *root = NULL;
	for (ClusterPool::iterator it = pool.begin(); it != pool.end();)
	{
		if ((clusterid) (*it->second) == cid)
		{
			if (it->second->IsRoot())
				root = it->second;
			else
				delete it->second;
			pool.erase(it);
			vs.push_back(it->first);
		}
		else
		{
			it++;
		}
	}
	if (root)
		delete root;
	
	return vs;
}

//------------------------------------------------------------------------------

bool GraphManager::Data::InCluster(const vertex &v) const
{
	return (pool.count(v) == 1);
}

//------------------------------------------------------------------------------

void GraphManager::Data::MergeCluster(const vertex &v1, const vertex &v2)
{
	(*pool[v1]) += pool[v2];
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
/*
void GraphManager::MergeClusters(int startCluster)
{
	std::vector<vector<vertex>>::size_type i = startCluster;
	while (i < clusters.size())
	{
		vector<vertex> currentCluster = clusters[i];
		vector<vertex> currentClusterConnections;
		int mergeWith = -1;
		for (vector<vertex>::size_type j = 0; j < currentCluster.size(); j++)
		{
			for (pair<int,int> k : graph.at(clusters[i][j]))
			{
				if (k.second>0)
				{
					int clusterOfk = FindClusterIndex(k.first);
					if (clusterOfk != i)
					{
						mergeWith = clusterOfk;
					}
					if (mergeWith >= 0)
					{
						break;
					}
				}
			}
			if (mergeWith >= 0)
			{
				break;
			}
		}
		
		if (mergeWith >= 0)
		{
			UnionClusters(i, mergeWith);

			if (mergeWith < i)
			{
				i--;
			}
		}
		else
		{
			i++;
		}
	}
}

//------------------------------------------------------------------------------

void GraphManager::RemakeClusters(int c1, int c2)
{
	vector<vertex> theCluster1(clusters[c1]);
	vector<vertex> theCluster2(clusters[c2]);
	bool sameCluster = c1 == c2;

	if (c1 > c2)
	{
		clusters.erase(clusters.begin() + c1);
		clusters.erase(clusters.begin() + c2);
	}
	else if (!sameCluster)
	{
		clusters.erase(clusters.begin() + c2);
		clusters.erase(clusters.begin() + c1);
	}
	else
	{
		clusters.erase(clusters.begin() + c1);
	}

	for (std::vector<vertex>::size_type i = 0; i != theCluster1.size(); i++)
	{
		vector<vertex> newCluster;
		newCluster.push_back(theCluster1[i]);
		clusters.push_back(newCluster);
	}
	if (!sameCluster)
	{
		for (std::vector<vertex>::size_type i = 0; i != theCluster2.size(); i++)
		{
			vector<vertex> newCluster;
			newCluster.push_back(theCluster2[i]);
			clusters.push_back(newCluster);
		}
	}

	int startindex = clusters.size()-theCluster1.size();
	if (!sameCluster)
	{
		startindex -= theCluster2.size();
	}
	MergeClusters(startindex);
}

//------------------------------------------------------------------------------

void GraphManager::UnionClusters(int c1, int c2)
{
	if (c1 == c2)
	{
		return;
	}
	vector<vertex> newCluster;
	newCluster.reserve(clusters[c1].size() + clusters[c2].size());
	newCluster.insert(newCluster.end(), clusters[c1].begin(), clusters[c1].end());
	newCluster.insert(newCluster.end(), clusters[c2].begin(), clusters[c2].end());

	if (c1 > c2)
	{
		clusters.erase(clusters.begin() + c1);
		clusters.erase(clusters.begin() + c2);
	}
	else
	{
		clusters.erase(clusters.begin() + c2);
		clusters.erase(clusters.begin() + c1);
	}

	clusters.push_back(newCluster);
}

//------------------------------------------------------------------------------

void GraphManager::AddEdgeToGraph(const Edge &e)
{
	if (graph.count(e.v1) == 0)
	{
		graph.insert({ e.v1, {} });
	}

	if (graph.count(e.v2) == 0)
	{
		graph.insert({ e.v2, {} });
	}

	if (graph.at(e.v1).count(e.v2) == 0)
	{
		graph.at(e.v1).insert({ e.v2, 1 });
	}
	else
	{
		graph.at(e.v1).at(e.v2) += 1;
	}

	if (graph.at(e.v2).count(e.v1) == 0)
	{
		graph.at(e.v2).insert({ e.v1, 1 });
	}
	else
	{
		graph.at(e.v2).at(e.v1) += 1;
	}
}

//------------------------------------------------------------------------------

void GraphManager::removeFromCluster(int v)
{
	vector< vector<vertex> >::size_type cluster = -1;
	vector<vertex>::size_type location = -1;
	for (std::vector< vector<vertex> >::size_type i = 0; i != clusters.size(); i++)
	{
		for (std::vector<vertex>::size_type j = 0; j != clusters[i].size(); j++)
		{
			if (clusters[i][j] == v)
			{
				cluster = i;
				location = j;
				break;
			}
		}
		if (cluster != -1)
		{
			break;
		}
	}
	clusters.at(cluster).erase(clusters.at(cluster).begin() + location);
	if (clusters.at(cluster).size() == 0)
	{
		clusters.erase(clusters.begin() + cluster);
	}
}
*/
//------------------------------------------------------------------------------

void GraphManager::Add(const Edge &e)
{
	/*
	//update theGraph
	AddEdgeToGraph(e);
	int cluster_v1 = FindClusterIndex(e.v1);

	//update theClusters
	if (cluster_v1<0)
	{
		vector<vertex> newCluster;
		newCluster.push_back(e.v1);
		clusters.push_back(newCluster);
		cluster_v1 = clusters.size() - 1;
	}

	int cluster_v2 = FindClusterIndex(e.v2);
	if (cluster_v2<0)
	{
		vector<vertex> newCluster;
		newCluster.push_back(e.v2);
		clusters.push_back(newCluster);
		cluster_v2 = clusters.size() - 1;
	}

	if (cluster_v1 != cluster_v2)
	{
		UnionClusters(cluster_v1, cluster_v2);
	}
	*/
}

//------------------------------------------------------------------------------

void GraphManager::RemoveExact(const Edge &e)
{
	Remove(e);
}

//------------------------------------------------------------------------------

void GraphManager::Remove(const Edge &e)
{
	/*
	graph.at(e.v1).at(e.v2) -= 1;
	graph.at(e.v2).at(e.v1) -= 1;
	bool remakeC1 = true;
	bool remakeC2 = true;

	if (graph.at(e.v1).at(e.v2) == 0)
	{
		graph.at(e.v1).erase(e.v2);
		if (graph.at(e.v1).size() == 0)
		{
			graph.erase(e.v1);
			removeFromCluster(e.v1);
			remakeC1 = false;
		}
	}

	if (e.v1 != e.v2)
	{
		if (graph.at(e.v2).at(e.v1) == 0)
		{
			graph.at(e.v2).erase(e.v1);
			if (graph.at(e.v2).size() == 0)
			{
				graph.erase(e.v2);
				removeFromCluster(e.v2);
				remakeC2 = false;
			}
		}
	}

	if (remakeC1 && remakeC2)
	{
		if (e.v1 != e.v2)
		{
			RemakeClusters(FindClusterIndex(e.v1), FindClusterIndex(e.v2));
		}
		else
		{
			int i = FindClusterIndex(e.v1);
			RemakeClusters(i, i);
		}
	}
	else
	{
		if (remakeC1)
		{
			int i = FindClusterIndex(e.v1);
			RemakeClusters(i,i);
		}
		else if (remakeC2)
		{
			int i = FindClusterIndex(e.v2);
			RemakeClusters(i, i);
		}
	}
	*/
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
	for (ClusterPool::iterator it = data->pool.begin(); it != data->pool.end(); ++it)
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
