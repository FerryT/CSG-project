#ifndef TESTS_H
#define TESTS_H

#include "Graph.h"
#include "Algorithm.h"
#include "GraphInput.h"

//------------------------------------------------------------------------------

class Test {
	public:
		Algorithm* algorithm;
		Input* input;
		// Runs the tests and writes the results to an output file in CSV
		virtual void RunTest(string outputFile) = 0;
};

//------------------------------------------------------------------------------

class QualityTest: public Test {
	public:
		int SnapshotSize = 10000;
		virtual void RunTest(string outputFile) override;
};

//------------------------------------------------------------------------------

class QualityTestCapture : public Algorithm
{
	private:
		Graph* _currentGraph;
	
	public:
		Algorithm* RealAlgorithm;
	
		QualityTestCapture();
		~QualityTestCapture();
		
		virtual void Add(Edge e) override;
		virtual void Remove(Edge e) override;
	
		//The query methods:
		virtual int FindClusterIndex(vertex u) override;
		virtual vector<vertex> FindCluster(vertex u) override;
		virtual int CountClusters() override;
	
		Graph* GetCompleteGraph();
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
