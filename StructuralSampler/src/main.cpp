
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

int main(int argc, char *argv[])
{
	srand(time(NULL));

	try
	{
		vector<ComponentDescription> arguments = GetDescriptions(argc, argv);

		//Input* input = CreateInput(*find_if(arguments.begin(), arguments.end(), IsType<ComInput>));
		//Algorithm* alg = CreateAlgorithm(*find_if(arguments.begin(), arguments.end(), IsType<ComAlgorithm>));
		Test* test = CreateTest(*find_if(arguments.begin(), arguments.end(), IsType<ComTest>));
		string outputfilename = GetOutputFilename(*find_if(arguments.begin(), arguments.end(), IsType<ComOutput>));
		
		/*vector<StackInput*> stackInputs;
		auto item = arguments.begin();
		item = find_if(item, arguments.end(), IsType<ComStackInput>);
		while (item != arguments.end())
		{
			StackInput* stackInput = CreateStackInput(*item);
			stackInputs.push_back(stackInput);

			item = find_if(item+1, arguments.end(), IsType<ComStackInput>);
		}*/

		
		//test->algorithm = alg;
		//test->input = input;
		//test->stackinputs = stackInputs;
		test->descriptions = arguments;
		test->RunTest(outputfilename);
	}
	catch (const char *msg) 
	{
		fprintf(stderr, "Fatal error:\n\t%s\n", msg);
		cout << "Basic usage: StructuralSampler -CI <input> [parameters] -CA <algorithm> [parameters] -CT <test> [parameters] -CO <output file>" << endl;
		cout << "Example usage: StructuralSampler -CI Poisson 100 0.80 400 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO testresult.png" << endl;
		getchar();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
