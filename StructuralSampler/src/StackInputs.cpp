#include "StackInputs.h"

void FilterEdges::Add(Edge e)
{
	if (e.p > this->Filter)
	{
		StackInput::Add(e);
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
