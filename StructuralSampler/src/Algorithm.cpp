#include <stdlib.h>
#include <time.h>

#include "Algorithm.h"

//------------------------------------------------------------------------------

StructuralSampler::StructuralSampler(int maxClusterSize) : manager(maxClusterSize)
{
	srand(time(NULL));
}

//------------------------------------------------------------------------------

void StructuralSampler::Add(edge newEdge)
{
	//TODO: sampling method, mentioned under C. 3)
	double pos = (double) rand() / (double) RAND_MAX;
	newEdge.p = pos;
	strReservoir.Add(newEdge);
	manager.Add(newEdge);
	if (!manager.ConstraintSatisfied())
	{
		edge currEdge;
		while (!manager.ConstraintSatisfied())
		{
			currEdge = strReservoir.RemoveLast();
			pos = currEdge.p;
			supReservoir.Add(currEdge);
			manager.RemoveExact(currEdge);
		}
		edge * searchResults = supReservoir.GetEdges(pos);
		int searchResultsLength = supReservoir.lastGetEdgesLength;
		for (int i = 0; i < searchResultsLength; i++)
		{
			currEdge = searchResults[i];
			manager.Add(currEdge);
			if (!manager.ConstraintSatisfied())
			{
				manager.RemoveExact(currEdge);
			}
			else
			{
				supReservoir.RemoveExact(currEdge);
				strReservoir.Add(currEdge);
			}
		}
	}
}

//------------------------------------------------------------------------------

void StructuralSampler::Remove(edge theEdge)
{
	bool supSucces = supReservoir.Remove(theEdge);
	if (strReservoir.HasEdge(theEdge))
	{
		double rmEdgeP = strReservoir.Remove(theEdge);
		manager.Remove(theEdge);
		edge * searchResults = supReservoir.GetEdges(rmEdgeP);
		int searchResultsLength = supReservoir.lastGetEdgesLength;
		edge currEdge;
		for (int i = 0; i < searchResultsLength; i++)
		{
			currEdge = searchResults[i];
			manager.Add(currEdge);
			if (!manager.ConstraintSatisfied())
			{
				manager.RemoveExact(currEdge);
			}
			else
			{
				supReservoir.RemoveExact(currEdge);
				strReservoir.Add(currEdge);
			}
		}
	}
}

//------------------------------------------------------------------------------
