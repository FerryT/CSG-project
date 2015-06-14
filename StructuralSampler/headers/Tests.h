#ifndef TESTS_H
#define TESTS_H

#include <ctime>

#include "Graph.h"
#include "Algorithm.h"
#include "GraphInput.h"
#include "ComponentDescription.h"

//------------------------------------------------------------------------------

class Test {
	public:
		Algorithm* algorithm;
		ComponentDescriptions descriptions;
		Input* input;
		std::vector<StackInput*> stackinputs;
		// Runs the tests and writes the results to an output file in CSV
		virtual void RunTest(String outputFile) = 0;
		//parses arguments for configuration
		virtual void ParseArguments(const Strings& arguments);
};

//------------------------------------------------------------------------------

class QualityTest: public Test {
	public:
		void ParseArguments(const Strings& arguments);
		QualityTest() : SnapshotSize(10000) {};
		QualityTest(int snapshotSize) : SnapshotSize(snapshotSize) {};
		int SnapshotSize;
		virtual void RunTest(String outputFile);
};

//------------------------------------------------------------------------------

class CaptureStackInput : public StackInput
{
	private:
		Graph* _currentGraph;
	
	public:
		CaptureStackInput();
		~CaptureStackInput();
		
		virtual void Add(Edge e);
		virtual void Remove(Edge e);
	
		Graph* GetCompleteGraph();
};


class SplitStackInput : public StackInput
{
private: 
	std::vector<Output*> outputs;
	bool is_open;
public:
	SplitStackInput() : is_open(false) {}

	void SetOutput(Output* output);
	void Open();
	void Close();
	void ExecuteNextUpdate();
	bool IsEnd();
	void Add(Edge e);
	void Remove(Edge e);
};

class ThroughputTest : public Test {
public:
	void ParseArguments(const Strings& arguments);
	ThroughputTest() : updates(5), queries(1) {}
	ThroughputTest(int updates, int queries) : updates(updates), queries(queries) {};
	int updates;
	int queries;
	virtual void RunTest(String outputFile);
};

//------------------------------------------------------------------------------

#endif // TESTS_H
