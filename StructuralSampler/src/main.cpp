
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstring>

#include "GraphInput.h"
#include "Algorithm.h"
#include "RandomizedInput.h"
#include "Tests.h"
#include "Visualize.h"
#include "WindowManager.h"
#include "StackInputs.h"
#include "CommandLineHelper.h"
#include "ConsoleColor.h"

int main(int argc, char *argv[])
{
	srand((unsigned int) time(NULL));

	try
	{
		ComponentDescriptions arguments = GetDescriptions(argc, argv);

		auto testDescription = find_if(arguments.begin(), arguments.end(), IsType<ComTest>);
		if (testDescription == arguments.end())
			throw "Need atleast a single test in the command line";
		Test* test = CreateTest(*testDescription);

		auto outputDescription = find_if(arguments.begin(), arguments.end(), IsType<ComOutput>);
		if (outputDescription == arguments.end())
			throw "Need atleast a single output in the command line";
		String outputfilename = GetOutputFilename(*outputDescription);
		
		test->descriptions = arguments;
		test->RunTest(outputfilename);
	}
	catch (const char *msg) 
	{
		cerr << red << "Fatal error: " << msg << white << endl;
		cout << "Basic usage: StructuralSampler -CI <input> [parameters] -CA <algorithm> [parameters] -CT <test> [parameters] -CO <output file>" << endl;
		cout << "Example usage: StructuralSampler -CI Poisson 100 0.80 400 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO testresult.png" << endl;
		return EXIT_FAILURE;
	}
	catch (exception& e)
	{
		cout << red << "Fatal error: " << e.what() << white << endl;
	}
	return EXIT_SUCCESS;
}
