#ifndef VISUALIZE_H
#define VISUALIZE_H

#include "Tests.h"
#include "GraphInput.h"

class OutputVisualization: public Test
{
public:
	bool CallDot;
	void RunTest(String outputFile);
};

class VisualizeResults : public Test
{
public:
	int runTillUpdate;
	bool CallDot;
	std::string* nameInput;

	VisualizeResults(int runTillUpdate = 100) : runTillUpdate(-1), CallDot(false), nameInput(NULL) {}
	void RunTest(String outputFile);
	void ParseArguments(const Strings& arguments);
};

class NullOutput: public Output
{
public:
	void Add(Edge e) {};
	void Remove(Edge e) {};
};



#endif // VISUALIZE_H
