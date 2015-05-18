
#include "ReservoirManager.h"

//------------------------------------------------------------------------------

void StructuralReservoir::Add(const Edge &e)
{
	//assume graph is sorted by p-values allready
	int location = 0;
	while (graph.at(location).p < e.p && location<graph.size())
	{
		location++;
	}
	graph.insert(graph.begin()+location, e);
}

//------------------------------------------------------------------------------

void StructuralReservoir::RemoveExact(const Edge &e)
{
	int location = 0;
	while ((graph.at(location) != e || graph.at(location).p != e.p) && location<graph.size())
	{
		location++;
	}
	if (location >= graph.size())
	{
		//e is not in the graph
		throw new exception("error while removing exact edge from structural reservoir");
	}
	graph.erase(graph.begin() + location);
}

//------------------------------------------------------------------------------

double StructuralReservoir::Remove(const Edge &e)
{
	int location = 0;
	while (graph.at(location) != e && location<graph.size())
	{
		location++;
	}
	if (location >= graph.size())
	{
		//e is not in the graph
		throw new exception("error while removing exact edge from structural reservoir");
	}
	graph.erase(graph.begin() + location);
}

//------------------------------------------------------------------------------

Edge StructuralReservoir::RemoveLast()
{
	Edge result = graph.at(graph.size() - 1);
	graph.pop_back();
	return result;
}

//------------------------------------------------------------------------------

bool StructuralReservoir::HasEdge(const Edge &e)
{
	for (vector<Edge>::size_type i = 0; i < graph.size(); i++)
	{
		if (graph[i] == e)
		{
			return true;
		}
	}
	return false;
}

//==============================================================================

void SupportReservoir::Add(const Edge &e)
{
	//assume graph is sorted by p-values allready
	int location = 0;
	while (graph.at(location).p < e.p && location<graph.size())
	{
		location++;
	}
	graph.insert(graph.begin() + location, e);
}

//------------------------------------------------------------------------------

void SupportReservoir::RemoveExact(const Edge &e)
{
	int location = 0;
	while ((graph.at(location) != e || graph.at(location).p != e.p) && location<graph.size())
	{
		location++;
	}
	if (location >= graph.size())
	{
		//e is not in the graph
		throw new exception("error while removing exact edge from structural reservoir");
	}
	graph.erase(graph.begin() + location);
}

//------------------------------------------------------------------------------

bool SupportReservoir::Remove(const Edge &e)
{
	int location = 0;
	while (graph.at(location) != e && location<graph.size())
	{
		location++;
	}
	if (location >= graph.size())
	{
		//e is not in the graph
		throw new exception("error while removing exact edge from structural reservoir");
	}
	graph.erase(graph.begin() + location);
}

//------------------------------------------------------------------------------

vector<Edge> SupportReservoir::GetEdges(double minP)
{
	int start = 0;

	while (graph.at(start).p < minP && start < graph.size())
	{
		start++;
	}
	vector<Edge> result;
	while (start < graph.size())
	{
		result.push_back(graph.at(start));
	}
	return result;
}

//------------------------------------------------------------------------------
