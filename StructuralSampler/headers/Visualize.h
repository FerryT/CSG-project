#ifndef VISUALIZE_H
#define VISUALIZE_H

#include "Tests.h"
#include "GraphInput.h"

class OutputVisualization: public Test
{
public:
	bool CallDot = false;
	void RunTest(string outputFile) override;
};

class VisualizeResults : public Test
{
public:
	int runTillUpdate = 10000;
	bool CallDot = false;
	VisualizeResults(int runTillUpdate);
	void RunTest(string outputFile) override;
};

class NullOutput: public Output
{
public:
	void Add(Edge e) override {};
	void Remove(Edge e) override {};
};



#endif // VISUALIZE_H
