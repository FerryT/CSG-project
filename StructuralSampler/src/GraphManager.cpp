
#include "GraphManager.h"

//------------------------------------------------------------------------------

void GraphManager::Add(const edge &e)
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

	UpdateUnionFind();
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
	UpdateUnionFind();
}

//------------------------------------------------------------------------------

void GraphManager::UpdateUnionFind()
{
	//TODO
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
	//TODO
	return 0;
}

//------------------------------------------------------------------------------

vector<vertex> GraphManager::FindCluster(vertex u)
{
	//TODO
	vector<vertex> result;
	return result;
}

//------------------------------------------------------------------------------

int GraphManager::CountClusters()
{
	//TODO
	return 0;
}

//------------------------------------------------------------------------------
