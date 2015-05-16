#include "GraphManager.h"

graphManager::graphManager()
{
	//theGraph = {};
	//TODO?
}

void graphManager::insertEdge(edge e)
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

	updateUnionFind();
}

void graphManager::removeEdgeExact(edge e)
{
	removeEdge(e);
}

void graphManager::removeEdge(edge e)
{
	theGraph.at(e.v1).at(e.v2) += -1;
	theGraph.at(e.v2).at(e.v1) += -1;
	updateUnionFind();
}

void graphManager::updateUnionFind()
{
	//TODO
}

bool graphManager::constraintSatisfied()
{
	//TODO
	return false;
}

int graphManager::FindClusterIndex(vertex u)
{
	//TODO
	return 0;
}

vector<vertex> graphManager::FindCluster(vertex u)
{
	//TODO
	vector<vertex> result;
	return result;
}

int graphManager::CountClusters()
{
	//TODO
	return 0;
}