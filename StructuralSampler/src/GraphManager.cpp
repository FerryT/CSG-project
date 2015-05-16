#include "GraphManager.h"

graphManager::graphManager(int B)
{
	maxClusterSize = B;
	//theGraph = {};
	//theClusters = {};
	//TODO?
}

void graphManager::mergeClusters(int startCluster)
{
	std::vector<vector<vertex>>::size_type i = startCluster;
	while (i < theClusters.size())
	{
		vector<vertex> currentCluster = theClusters[i];
		vector<vertex> currentClusterConnections;
		int mergeWith = -1;
		for (vector<vertex>::size_type j = 0; j < currentCluster.size(); j++)
		{
			for (pair<int,int> k : theGraph.at(theClusters[i][j]))
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

void graphManager::remakeClusters(int c1, int c2)
{
	vector<vertex> theCluster1(theClusters[c1]);
	vector<vertex> theCluster2(theClusters[c2]);
	bool sameCluster = c1 == c2;

	if (c1 > c2)
	{
		theClusters.erase(theClusters.begin() + c1);
		theClusters.erase(theClusters.begin() + c2);
	}
	else if (!sameCluster)
	{
		theClusters.erase(theClusters.begin() + c2);
		theClusters.erase(theClusters.begin() + c1);
	}
	else
	{
		theClusters.erase(theClusters.begin() + c1);
	}

	for (std::vector<vertex>::size_type i = 0; i != theCluster1.size(); i++)
	{
		vector<vertex> newCluster;
		newCluster.push_back(theCluster1[i]);
		theClusters.push_back(newCluster);
	}
	if (!sameCluster)
	{
		for (std::vector<vertex>::size_type i = 0; i != theCluster2.size(); i++)
		{
			vector<vertex> newCluster;
			newCluster.push_back(theCluster2[i]);
			theClusters.push_back(newCluster);
		}
	}

	int startindex = theClusters.size()-theCluster1.size();
	if (!sameCluster)
	{
		startindex -= theCluster2.size();
	}
	mergeClusters(startindex);
}

void graphManager::UnionClusters(int c1, int c2)
{
	if (c1 == c2)
	{
		return;
	}
	vector<vertex> newCluster;
	newCluster.reserve(theClusters[c1].size + theClusters[c2].size);
	newCluster.insert(newCluster.end(), theClusters[c1].begin(), theClusters[c1].end());
	newCluster.insert(newCluster.end(), theClusters[c2].begin(), theClusters[c2].end());

	if (c1 > c2)
	{
		theClusters.erase(theClusters.begin() + c1);
		theClusters.erase(theClusters.begin() + c2);
	}
	else
	{
		theClusters.erase(theClusters.begin() + c2);
		theClusters.erase(theClusters.begin() + c1);
	}

	theClusters.push_back(newCluster);
}

void graphManager::AddEdgeToGraph(edge e)
{
	if (theGraph.count(e.v1 == 0))
	{
		theGraph.insert({ e.v1, {} });
	}

	if (theGraph.count(e.v2 == 0))
	{
		theGraph.insert({ e.v2, {} });
	}

	if (theGraph.at(e.v1).count(e.v2) == 0)
	{
		theGraph.at(e.v1).insert({ e.v2, 1 });
	}
	else
	{
		theGraph.at(e.v1).at(e.v2) += 1;
	}

	if (theGraph.at(e.v2).count(e.v1) == 0)
	{
		theGraph.at(e.v2).insert({ e.v1, 1 });
	}
	else
	{
		theGraph.at(e.v2).at(e.v1) += 1;
	}
}

void graphManager::insertEdge(edge e)
{
	//update theGraph
	AddEdgeToGraph(e);
	int cluster_v1 = FindClusterIndex(e.v1);

	//update theClusters
	if (cluster_v1<0)
	{
		vector<vertex> newCluster;
		newCluster.push_back(e.v1);
		theClusters.push_back(newCluster);
		cluster_v1 = theClusters.size - 1;
	}

	int cluster_v2 = FindClusterIndex(e.v2);
	if (cluster_v2<0)
	{
		vector<vertex> newCluster;
		newCluster.push_back(e.v2);
		theClusters.push_back(newCluster);
		cluster_v2 = theClusters.size - 1;
	}

	if (cluster_v1 != cluster_v2)
	{
		UnionClusters(cluster_v1, cluster_v2);
	}
}

void graphManager::removeEdgeExact(edge e)
{
	removeEdge(e);
}

void graphManager::removeEdge(edge e)
{
	theGraph.at(e.v1).at(e.v2) += -1;
	theGraph.at(e.v2).at(e.v1) += -1;

	remakeCluster(FindClusterIndex(e.v1));
	remakeCluster(FindClusterIndex(e.v2));
}

bool graphManager::constraintSatisfied()
{
	for (std::vector<vector<vertex>>::size_type i = 0; i < theClusters.size(); i++)
	{
		if (theClusters[i].size()>maxClusterSize)
		{
			return false;
		}
	}
	return true;
}

int graphManager::FindClusterIndex(vertex u)
{
	for (std::vector<vector<vertex>>::size_type i =0; i != theClusters.size(); i++)
	{
		for (std::vector<vector<vertex>>::size_type j = 0; j != theClusters[i].size(); j++)
		{
			if (theClusters[i][j] == u)
			{
				return i;
			}
		}
	}

	return -1;
}

vector<vertex> graphManager::FindCluster(vertex u)
{
	return theClusters[FindClusterIndex(u)];
}

int graphManager::CountClusters()
{
	return theClusters.size();
}