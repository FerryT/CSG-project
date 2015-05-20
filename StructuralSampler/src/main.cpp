
#include <cstdlib>

#include "GraphInput.h" // debug
#include "Algorithm.h"
#include "RandomizedInput.h"
#include "Tests.h"
#include "Visualize.h"
#include <iostream>

int main(int argc, char *argv[])
{
	Input* input = new EulerNetworkInput(100, 0.75, 1000);
	Algorithm* alg = new StructuralSampler(10);
	Test* test = new QualityTest(100);
	test->algorithm = alg;
	test->input = input;
	test->RunTest("testResult.csv");

	/*Input* input = new ScaleFreeNetworkInput(100, 1, 200);
	OutputVisualization* test = new OutputVisualization();
	test->CallDot = true;
	test->input = input;
	test->RunTest("testGraph.png");*/

	return EXIT_SUCCESS;
/*	if (argc < 2)
	{
		printf("Usage:\n\tStructuralSampler <graph file>\t");
		return EXIT_FAILURE;
	}
	
	try {
		

		// debug [
		MGraphFileInput file(argv[1]);
		file.Open();
		while (!file.IsEnd())
			file.ExecuteNextUpdate();
		file.Close();
		// debug ]
	} catch (const char *msg) {
		fprintf(stderr, "Fatal error:\n\t%s\n", msg);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;*/
}
