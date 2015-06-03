#include <algorithm>
#include <StackInputs.h>
#include <CommandLineHelper.h>
#include <RandomizedInput.h>
#include <Visualize.h>
#include <WindowManager.h>


ComponentDescriptions GetDescriptions(int argc, char *argv[])
{
	Strings arguments(argv, argv+argc);
	return GetDescriptions(arguments);
}

ComponentDescriptions GetDescriptions(Strings argv)
{
	std::vector<int> componentStarts;
	typedef std::vector<int>::size_type size_type;
	ComponentDescriptions result;
	for (Strings::size_type i = 0; i < argv.size(); i++)
	{
		if (argv[i].compare("-CI") == 0 || argv[i].compare("-CS") == 0 || argv[i].compare("-CA") == 0 || argv[i].compare("-CT") == 0 || argv[i].compare("-CO") == 0)
		{
			componentStarts.push_back(i);
		}
	}

	for (size_type i = 0; i < componentStarts.size(); i++)
	{
		int start = componentStarts[i];
		int end;
		if (i == componentStarts.size() - 1)
			end = argv.size();
		else
			end = componentStarts[i + 1];

		ComponentDescription desc;
		if (argv[start].compare("-CI") == 0)
		{
			desc.type = Component::ComInput;
		}
		else if (argv[start].compare("-CS") == 0)
		{
			desc.type = Component::ComStackInput;
		}
		else if (argv[start].compare("-CA") == 0)
		{
			desc.type = Component::ComAlgorithm;
		}
		else if (argv[start].compare("-CT") == 0)
		{
			desc.type = Component::ComTest;
		}
		else if (argv[start].compare("-CO") == 0)
		{
			desc.type = Component::ComOutput;
		}
		else
		{
			throw "Parsing of command line went wrong";
		}

		if (((size_type) start + 1) >= argv.size())
		{
			throw "Wrong usage of application";
		}

		if (desc.type == Component::ComOutput)
		{
			desc.parameters.push_back(String(argv[start + 1]));
		}
		else
		{
			desc.name = String(argv[start + 1]);
			for (int j = start + 1 + 1; j < end; j++)
			{
				desc.parameters.push_back(String(argv[j]));
			}
		}
		result.push_back(desc);
	}
	return result;
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
	else if (desc.name == "BAModel")
	{
		result = new BAModelInput();
	}
	else if (desc.name == "mgraph")
	{
		result = new MGraphFileInput();
	}
	else if (desc.name == "graph")
	{
		result = new EdgeFileInput();
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
		result = new Metis(10);
	}
	else if (desc.name == "StructuralSampler")
	{
		result = new StructuralSampler();
	}
	else
	{
		throw "Algorithm does not exist";
	}

	result->ParseArguments(desc.parameters);

	return result;
}

vector<Algorithm*> CreateAlgorithms(ComponentDescriptions arguments)
{
	vector<Algorithm*> result;
	auto item = arguments.begin();
	item = find_if(item, arguments.end(), IsType<ComAlgorithm>);
	while (item != arguments.end())
	{
		Algorithm* stackInput = CreateAlgorithm(*item);
		result.push_back(stackInput);

		item = find_if(item + 1, arguments.end(), IsType<ComAlgorithm>);
	}
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
	else if (desc.name == "Throughput")
	{
		result = new ThroughputTest();
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
	else if (desc.name == "FilterEdges")
	{
		result = new FilterEdges();
	}
	else
	{
		throw "StackInput does not exist";
	}

	result->ParseArguments(desc.parameters);

	return result;
}

vector<StackInput*> CreateStackInputs(ComponentDescriptions arguments)
{
	vector<StackInput*> stackInputs;
	auto item = arguments.begin();
	item = find_if(item, arguments.end(), IsType<ComStackInput>);
	while (item != arguments.end())
	{
		StackInput* stackInput = CreateStackInput(*item);
		stackInputs.push_back(stackInput);

		item = find_if(item + 1, arguments.end(), IsType<ComStackInput>);
	}
	return stackInputs;
}

String GetOutputFilename(ComponentDescription desc)
{
	if (desc.type != ComOutput)
		throw "Not an output";

	return desc.parameters[0];
}
