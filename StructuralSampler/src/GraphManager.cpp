
#include "GraphManager.h"

//------------------------------------------------------------------------------

void GraphManager::MergeClusters()
{
	//TODO
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

	MergeClusters();
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

void GraphManager::AddEdgeToGraph(const edge &e)
{
	if (g.count(e.v1 == 0))
	{
		g.insert({ e.v1, {} });
	}

	if (g.count(e.v2 == 0))
	{
		g.insert({ e.v2, {} });
	}

	if (g.at(e.v1).count(e.v2) == 0)
	{
		g.at(e.v1).insert({ e.v2, 1 });
	}
	else
	{
		g.at(e.v1).at(e.v2) += 1;
	}

	if (g.at(e.v2).count(e.v1) == 0)
	{
		g.at(e.v2).insert({ e.v1, 1 });
	}
	else
	{
		g.at(e.v2).at(e.v1) += 1;
	}
}

//------------------------------------------------------------------------------

void GraphManager::Add(const edge &e)
{
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
}

//------------------------------------------------------------------------------

void GraphManager::RemoveExact(const edge &e)
{
	Remove(e);
}

//------------------------------------------------------------------------------

void GraphManager::Remove(const edge &e)
{
	g.at(e.v1).at(e.v2) += -1;
	g.at(e.v2).at(e.v1) += -1;

	RemakeCluster(FindClusterIndex(e.v1));
	RemakeCluster(FindClusterIndex(e.v2));
}

//------------------------------------------------------------------------------

bool GraphManager::ConstraintSatisfied()
{
	//TODO
	return false;
}

//------------------------------------------------------------------------------

int GraphManager::FindClusterIndex(vertex u)
{
	for (std::vector< vector<vertex> >::size_type i =0; i != clusters.size(); i++)
	{
		for (std::vector< vector<vertex> >::size_type j = 0; j != clusters[i].size(); j++)
		{
			if (clusters[i][j] == u)
			{
				return i;
			}
		}
	}

	return -1;
}

//------------------------------------------------------------------------------

vector<vertex> GraphManager::FindCluster(vertex u)
{
	return clusters[FindClusterIndex(u)];
}

//------------------------------------------------------------------------------

int GraphManager::CountClusters()
{
	return clusters.size();
}

//------------------------------------------------------------------------------
