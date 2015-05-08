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


/*

//The classes that has to be implemented

class QualityTest {
public:
	Algorithm* algorithm;
	Input* input;
	virtual void RunTest(string outputFile);
};

class ThroughputTest {
public:
	Algorithm* algorithm;
	Input* input;
	virtual void RunTest(string outputFile);
};
*/

#endif