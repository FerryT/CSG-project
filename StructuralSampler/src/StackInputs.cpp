#include "StackInputs.h"

void FilterEdges::Remove(Edge e)
{
	Graph::iterator it = find(this->g.begin(), this->g.end(), e);
	if (it != this->g.end())
	{
		this->g.erase(it);
		StackInput::Remove(e);
	}
}

void FilterEdges::Add(Edge e)
{
	if (e.p > this->Filter)
	{
		StackInput::Add(e);
		this->g.push_back(e);
	}
}

void FilterEdges::ParseArguments(const vector<string>& arguments)
{
	if (arguments.size() == 0)
	{
		return;
	}
	else if (arguments.size() == 1)
	{
		this->Filter = atof(arguments[0].c_str());
	}
	else
	{
		throw "Can't parse the parameters for filter stack input, the only argument is the filter value";
	}
}
