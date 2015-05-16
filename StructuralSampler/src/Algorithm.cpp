#include <stdlib.h>
#include <time.h>

#include "Algorithm.h"

StructuralSampler::StructuralSampler()
{
	srand(time(NULL));
}

void StructuralSampler::Add(edge newEdge, structuralReservoir strReservoir, supportReservoir supReservoir, graphManager manager)
{
	//TODO: sampling method, mentioned under C. 3)
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
			manager.removeEdgeExact(currEdge);
		}
		edge * searchResults = supReservoir.getEdges(pos);
		int searchResultsLength = supReservoir.lastGetEdgesLength;
		for (int i = 0; i < searchResultsLength; i++)
		{
			currEdge = searchResults[i];
			manager.insertEdge(currEdge);
			if (!manager.constraintSatisfied())
			{
				manager.removeEdgeExact(currEdge);
			}
			else
			{
				supReservoir.removeEdgeExact(currEdge);
				strReservoir.insertEdge(currEdge);
			}
		}
	}
}

void StructuralSampler::Remove(edge theEdge, structuralReservoir strReservoir, supportReservoir supReservoir, graphManager manager)
{
	bool supSucces = supReservoir.removeEdge(theEdge);
	if (strReservoir.hasEdge(theEdge))
	{
		float rmEdgeP = strReservoir.removeEdge(theEdge);
		manager.removeEdge(theEdge);
		edge * searchResults = supReservoir.getEdges(rmEdgeP);
		int searchResultsLength = supReservoir.lastGetEdgesLength;
		edge currEdge;
		for (int i = 0; i < searchResultsLength; i++)
		{
			currEdge = searchResults[i];
			manager.insertEdge(currEdge);
			if (!manager.constraintSatisfied())
			{
				manager.removeEdgeExact(currEdge);
			}
			else
			{
				supReservoir.removeEdgeExact(currEdge);
				strReservoir.insertEdge(currEdge);
			}
		}
	}
}

