#include <algorithm>

#include "StackInputs.h"

void FilterEdges::Remove(Edge e)
{
	Graph::iterator it = graph.find(e);
	if (it != graph.end())
	{
		graph.erase(it);
		StackInput::Remove(e);
	}
}

void FilterEdges::Add(Edge e)
{
	if (e.p > Filter)
	{
		StackInput::Add(e);
		graph.insert(e);
	}
}

void FilterEdges::ParseArguments(const Strings& arguments)
{
	if (arguments.size() == 0)
	{
		return;
	}
	else if (arguments.size() == 1)
	{
		Filter = atof(arguments[0].c_str());
	}
	else
	{
		throw "Can't parse the parameters for filter stack input, the only argument is the filter value";
	}
}
