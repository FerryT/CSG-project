#ifndef TESTS_H
#define TESTS_H

#include "Graph.h"
#include "Algorithm.h"
#include "GraphInput.h"
#include <iostream>
#include <ctime>
#include "ComponentDescription.h"

//------------------------------------------------------------------------------

class Test {
	public:
		Algorithm* algorithm;
		vector<ComponentDescription> descriptions;
		Input* input;
		vector<StackInput*> stackinputs;
		// Runs the tests and writes the results to an output file in CSV
		virtual void RunTest(string outputFile) = 0;
		//parses arguments for configuration
		virtual void ParseArguments(const vector<string>& arguments) { std::cout << "arguments for this test are not yet parsed" << std::endl; }
};

//------------------------------------------------------------------------------

class QualityTest: public Test {
	public:
		void ParseArguments(const vector<string>& arguments) override;
		QualityTest();
		QualityTest(int snapshotSize);
		int SnapshotSize = 10000; 
		virtual void RunTest(string outputFile) override;
};

//------------------------------------------------------------------------------

class CaptureStackInput : public StackInput
{
	private:
		Graph* _currentGraph;
	
	public:
		CaptureStackInput();
		~CaptureStackInput();
		
		virtual void Add(Edge e) override;
		virtual void Remove(Edge e) override;
	
		Graph* GetCompleteGraph();
};


class SplitStackInput : public StackInput
{
private: 
	vector<Output*> outputs;
	bool is_open = false;
public:
	void SetOutput(Output* output) override;
	void Open() override;
	void Close() override;
	void ExecuteNextUpdate() override;
	bool IsEnd() override;
	void Add(Edge e) override;
	void Remove(Edge e) override;
};

/*

//The classes that has to be implemented



class ThroughputTest {
public:
	void RunTest(string outputFile);
};
*/

//------------------------------------------------------------------------------

#endif // TESTS_H
