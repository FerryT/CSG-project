#ifndef STACK_INPUTS_H
#define STACK_INPUTS_H

#include "GraphInput.h"

class FilterEdges : public StackInput
{
public:
	void Add(Edge e) override;
	double Filter = 0;
	void ParseArguments(const vector<string>& arguments) override;
	

};
#endif // TESTS_H
