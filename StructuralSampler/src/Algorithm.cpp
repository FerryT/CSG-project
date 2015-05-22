#include <stdlib.h>
#include <time.h>

#include "Algorithm.h"

//------------------------------------------------------------------------------

StructuralSampler::StructuralSampler(int maxClusterSize) : manager(maxClusterSize)
{
}

//------------------------------------------------------------------------------

StructuralSampler::StructuralSampler() : manager(10)
{
}

//------------------------------------------------------------------------------

void StructuralSampler::Add(Edge newEdge)
{
	//TODO: sampling method, mentioned under C. 3)
	double pos = (double) rand() / (double) RAND_MAX;
	newEdge.p = pos;
	strReservoir.Add(newEdge);
	manager.Add(newEdge);
	if (!manager.ConstraintSatisfied())
	{
		Edge currEdge;
		while (!manager.ConstraintSatisfied())
		{
			currEdge = strReservoir.RemoveLast();
			pos = currEdge.p;
			supReservoir.Add(currEdge);
			manager.RemoveExact(currEdge);
		}
		vector<Edge> searchResults = supReservoir.GetEdges(pos);
		for (vector<Edge>::size_type i = 0; i < searchResults.size(); i++)
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

void StructuralSampler::Remove(Edge theEdge)
{
	bool supSucces = supReservoir.Remove(theEdge);
	if (strReservoir.HasEdge(theEdge))
	{
		double rmEdgeP = strReservoir.Remove(theEdge);
		manager.Remove(theEdge);
		vector<Edge> searchResults = supReservoir.GetEdges(rmEdgeP);
		Edge currEdge;
		for (vector<Edge>::size_type i = 0; i < searchResults.size(); i++)
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

void StructuralSampler::ParseArguments(const vector<string>& arguments)
{
	if (arguments.size() == 0)
	{
		return;
	}
	else if (arguments.size() == 1)
	{
		this->manager.maxClusterSize = atoi(arguments[0].c_str());
	}
	else
	{
		throw "Can't parse the parameters for quality test the only argument is the snapshot size";
	}
}

//------------------------------------------------------------------------------
