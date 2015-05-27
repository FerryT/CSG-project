
#include "ReservoirManager.h"

//------------------------------------------------------------------------------

void StructuralReservoir::Add(const Edge &e)
{
	//assume graph is sorted by p-values allready
	if (graph.size() == 0)
	{
		graph.push_back(e);
	}
	else
	{
		int location = 0;
		while (location<graph.size())
		{
			if (graph.at(location).p >= e.p)
			{
				break;
			}
			location++;
		}
		graph.insert(graph.begin()+location, e);
	}
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
		throw "error while removing exact edge from structural reservoir";
	}
	graph.erase(graph.begin() + location);
}

//------------------------------------------------------------------------------

double StructuralReservoir::Remove(const Edge &e)
{
	int location = 0;
	while (graph[location] != e && location<graph.size())
	{
		location++;
	}
	if (location >= graph.size())
	{
		//e is not in the graph
		throw "error while removing exact edge from structural reservoir";
	}
	double p = graph[location].p;
	graph.erase(graph.begin() + location);
	return p;
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
	if (graph.size() == 0)
	{
		graph.push_back(e);
	}
	else
	{
		int location = 0;
		while (location<graph.size())
		{
			if (!(graph.at(location).p < e.p))
			{
				break;
			}
			location++;
		}
		graph.insert(graph.begin() + location, e);
	}
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
		throw "error while removing exact edge from structural reservoir";
	}
	graph.erase(graph.begin() + location);
}

//------------------------------------------------------------------------------

bool SupportReservoir::Remove(const Edge &e)
{
	int location = 0;
	while (location < graph.size())
	{
		if (graph.at(location) == e)
		{
			break;
		}
		location++;
	}
	if (location >= graph.size())
	{
		//e is not in the graph
		return false;
	}
	else
	{
		graph.erase(graph.begin() + location);
		return true;
	}
}

//------------------------------------------------------------------------------

vector<Edge> SupportReservoir::GetEdges(double minP)
{
	int start = 0;

	while (start < graph.size())
	{
		if (graph.at(start).p >= minP)
		{
			break;
		}
		start++;
	}
	vector<Edge> result;
	while (start < graph.size())
	{
		result.push_back(graph.at(start));
		start++;
	}
	return result;
}

//------------------------------------------------------------------------------
