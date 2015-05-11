#ifndef TESTS_H
#define TESTS_H

#include "Algorithm.h"
#include "GraphInput.h"

class Test {
public:
	Algorithm* algorithm;
	Input* input;
	//Runs the tests and writes the results to an output file in CSV
	virtual void RunTest(string outputFile) = 0;
};


class QualityTest: Test {
public:
	int SnapshotSize = 10000;
	virtual void RunTest(string outputFile) override;
};

/*

//The classes that has to be implemented



class ThroughputTest {
public:
	void RunTest(string outputFile);
};
*/

#endif