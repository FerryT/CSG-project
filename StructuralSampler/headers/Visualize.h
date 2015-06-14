#ifndef VISUALIZE_H
#define VISUALIZE_H

#include "Tests.h"
#include "GraphInput.h"

class OutputVisualization: public Test
{
public:
	bool CallDot = false;
	void RunTest(String outputFile);
};

class VisualizeResults : public Test
{
public:
	int runTillUpdate;
	bool CallDot;
	std::string* nameInput;

	VisualizeResults() : VisualizeResults(100){};
	VisualizeResults(int runTillUpdate) : runTillUpdate(-1), CallDot(false), nameInput(nullptr) {}
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
