
#include <cstdlib>

#include "GraphInput.h" // debug
#include "Algorithm.h"
#include "RandomizedInput.h"
#include "Tests.h"
#include "Visualize.h"
#include <iostream>
#include <algorithm>
#include <WindowManager.h>

enum Component { ComInput, ComStackInput, ComAlgorithm, ComTest, ComOutput };

struct ComponentDescription
{
	Component type;
	string name;
	vector<string> parameters;
};

vector<ComponentDescription> GetDescriptions(int argc, char *argv[])
{
	vector<int> componentStarts;
	vector<ComponentDescription> result;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-CI") == 0 || strcmp(argv[i], "-CS") == 0 || strcmp(argv[i], "-CA") == 0 || strcmp(argv[i], "-CT") == 0 || strcmp(argv[i], "-CO") == 0)
		{
			componentStarts.push_back(i);
		}
	}

	for (int i = 0; i < componentStarts.size(); i++)
	{
		int start = componentStarts[i];
		int end;
		if (i == componentStarts.size()-1)
			end = argc;
		else
			end = componentStarts[i+1];

		ComponentDescription desc;
		if (strcmp(argv[start], "-CI") == 0)
		{
			desc.type = Component::ComInput;
		}
		else if (strcmp(argv[start], "-CS") == 0)
		{
			desc.type = Component::ComStackInput;
		}
		else if (strcmp(argv[start], "-CA") == 0)
		{
			desc.type = Component::ComAlgorithm;
		}
		else if (strcmp(argv[start], "-CT") == 0)
		{
			desc.type = Component::ComTest;
		}
		else if (strcmp(argv[start], "-CO") == 0)
		{
			desc.type = Component::ComOutput;
		}
		else
		{
			throw "Parsing of command line went wrong";
		}

		if (start+1 >= argc)
		{
			throw "Wrong usage of application";
		}

		if (desc.type == Component::ComOutput)
		{
			desc.parameters.push_back(string(argv[start + 1]));
		}
		else
		{
			desc.name = string(argv[start + 1]);
			for (int j = start+1+1; j < end; j++)
			{
				desc.parameters.push_back(string(argv[j]));
			}
		}
		result.push_back(desc);
	}
	return result;
}

template<Component c>
bool IsType(ComponentDescription desc) { return desc.type == c; }

void correctConfiguration(vector<ComponentDescription> descriptions)
{
	if(count_if(descriptions.begin(), descriptions.end(), IsType<ComInput>) != 1)
		throw "Has to have exactly 1 input";

	if (count_if(descriptions.begin(), descriptions.end(), IsType<ComAlgorithm>) != 1)
		throw "Has to have exactly 1 algorithm";

	if (count_if(descriptions.begin(), descriptions.end(), IsType<ComTest>) != 1)
		throw "Has to have exactly 1 test";

	if (count_if(descriptions.begin(), descriptions.end(), IsType<ComOutput>) != 1)
		throw "Has to have exactly 1 output";
}

Input* CreateInput(ComponentDescription desc)
{
	if (desc.type != ComInput)
		throw "Not an input";

	Input* result;

	if (desc.name == "Poisson")
	{
		result = new EulerNetworkInput();
	}
	else if (desc.name == "Scale-Free")
	{
		result = new ScaleFreeNetworkInput();
	}
	else if (desc.name == "mgraph")
	{
		throw "Input does not work yet from command line";
		result = new MGraphFileInput("");
	}
	else if (desc.name == "graph")
	{
		throw "Input does not work yet from command line";
		result = new EdgeFileInput("");
	}
	else
	{
		throw "Input does not exist";
	}

	result->ParseArguments(desc.parameters);

	return result;
}

Algorithm* CreateAlgorithm(ComponentDescription desc)
{
	if (desc.type != ComAlgorithm)
		throw "Not an algorithm";

	Algorithm* result;

	if (desc.name == "Metis")
	{
		throw "Metis is not implemented yet.";
	}
	else if (desc.name == "StructuralSampler")
	{
		cout << "cluster size is set on 10" << endl;
		result = new StructuralSampler(10);
	}
	else
	{
		throw "Algorithm does not exist";
	}

	result->ParseArguments(desc.parameters);

	return result;
}

Test* CreateTest(ComponentDescription desc)
{
	if (desc.type != ComTest)
		throw "Not an algorithm";

	Test* result;

	if (desc.name == "Quality")
	{
		result = new QualityTest();
	}
	else if (desc.name == "VisualizeResult")
	{
		result = new VisualizeResults();
	}
	else
	{
		throw "Test does not exist";
	}

	result->ParseArguments(desc.parameters);

	return result;
}

StackInput* CreateStackInput(ComponentDescription desc)
{
	if (desc.type != ComStackInput)
		throw "Not an StackInput";

	StackInput* result;

	if (desc.name == "SlideWindow")
	{
		result = new SlideWindow();
	}
	else if (desc.name == "TumblingWindow")
	{
		result = new TumblingWindow();
	}
	else
	{
		throw "StackInput does not exist";
	}

	result->ParseArguments(desc.parameters);

	return result;
}

string GetOutputFilename(ComponentDescription desc)
{
	if (desc.type != ComOutput)
		throw "Not an output";

	return desc.parameters[0];
}

int main(int argc, char *argv[])
{
	try
	{
		vector<ComponentDescription> arguments = GetDescriptions(argc, argv);
		correctConfiguration(arguments);

		Input* input = CreateInput(*find_if(arguments.begin(), arguments.end(), IsType<ComInput>));
		Algorithm* alg = CreateAlgorithm(*find_if(arguments.begin(), arguments.end(), IsType<ComAlgorithm>));
		Test* test = CreateTest(*find_if(arguments.begin(), arguments.end(), IsType<ComTest>));
		string outputfilename = GetOutputFilename(*find_if(arguments.begin(), arguments.end(), IsType<ComOutput>));
		test->algorithm = alg;
		test->input = input;
		test->RunTest(outputfilename);
	}
	catch (const char *msg) 
	{
		fprintf(stderr, "Fatal error:\n\t%s\n", msg);
		cout << "Basic usage: StructuralSampler -CI <input> [parameters] -CA <algorithm> [parameters] -CT <test> [parameters] -CO <output file>";
		cout << "Example usage: StructuralSampler -CI Poisson 100 0.80 400 -CA StructuralSampler 10 -CT VisualizeResult 100 1 -CO testresult.png";
		getchar();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
