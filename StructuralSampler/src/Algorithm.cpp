#include <stdlib.h>
#include <time.h>

#include "Algorithm.h"

StructuralSampler::StructuralSampler()
{
	srand(time(NULL));
}

void StructuralSampler::Add(edge newEdge, structuralReservoir strReservoir, supportReservoir supReservoir, graphManager manager)
{
	float pos = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	newEdge.p = pos;
	strReservoir.insertEdge(newEdge);
	manager.insertEdge(newEdge);
	if (!manager.constraintSatisfied())
	{
		edge currEdge;
		while (!manager.constraintSatisfied())
		{
			currEdge = strReservoir.removeLast();
			pos = currEdge.p;
			supReservoir.insertEdge(currEdge);
			manager.removeEdge(currEdge);
		}
		edge * searchResults = supReservoir.getEdges(pos);
		int searchResultsLength = supReservoir.lastGetEdgesLength;
		for (int i = 0; i < searchResultsLength; i++)
		{
			currEdge = searchResults[i];
			manager.insertEdge(currEdge);
			if (!manager.constraintSatisfied())
			{
				manager.removeEdge(currEdge);
			}
			else
			{
				supReservoir.removeEdge(currEdge);
				strReservoir.insertEdge(currEdge);
			}
		}
	}
}

void StructuralSampler::Remove(edge e)
{
	//TODO
}

int StructuralSampler::FindClusterIndex(vertex u)
{
	//TODO
	return 0;
}

vector<vertex> StructuralSampler::FindCluster(vertex u)
{
	//TODO
	vector<vertex> result;
	return result;
}

int StructuralSampler::CountClusters()
{
	//TODO
	return 0;
}