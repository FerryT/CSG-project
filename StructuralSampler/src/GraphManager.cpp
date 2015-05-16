
#include "GraphManager.h"

//------------------------------------------------------------------------------

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

void GraphManager::AddEdgeToGraph(const edge &e)
{
	if (graph.count(e.v1 == 0))
	{
		graph.insert({ e.v1, {} });
	}

	if (graph.count(e.v2 == 0))
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
	graph.at(e.v1).at(e.v2) += -1;
	graph.at(e.v2).at(e.v1) += -1;

	// Todo: fix this (function does not exist?)
	//RemakeCluster(FindClusterIndex(e.v1));
	//RemakeCluster(FindClusterIndex(e.v2));
}

//------------------------------------------------------------------------------

bool GraphManager::ConstraintSatisfied()
{
	for (std::vector<vector<vertex>>::size_type i = 0; i < clusters.size(); i++)
	{
		if (clusters[i].size()>maxClusterSize)
		{
			return false;
		}
	}
	return true;
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
