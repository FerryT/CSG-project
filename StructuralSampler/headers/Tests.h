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
		void ParseArguments(const Strings& arguments) override;
		QualityTest();
		QualityTest(int snapshotSize);
		int SnapshotSize = 10000; 
		virtual void RunTest(String outputFile) override;
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
	std::vector<Output*> outputs;
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
	void RunTest(String outputFile);
};
*/

//------------------------------------------------------------------------------

#endif // TESTS_H
