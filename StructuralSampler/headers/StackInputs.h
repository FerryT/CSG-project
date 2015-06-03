#ifndef STACK_INPUTS_H
#define STACK_INPUTS_H

#include "GraphInput.h"

class FilterEdges : public StackInput
{
	Graph graph;
public:
	double Filter = 0;
	void Remove(Edge e) override;
	void Add(Edge e) override;
	void ParseArguments(const Strings& arguments) override;
	

};
#endif // TESTS_H
