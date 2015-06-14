#ifndef STACK_INPUTS_H
#define STACK_INPUTS_H

#include "GraphInput.h"

class FilterEdges : public StackInput
{
	Graph graph;
public:
	double Filter;

	FilterEdges() : Filter(0) {}
	void Remove(Edge e);
	void Add(Edge e);
	void ParseArguments(const Strings& arguments);
	

};
#endif // TESTS_H
