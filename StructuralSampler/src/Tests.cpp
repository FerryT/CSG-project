#include <iostream>
#include <fstream>
#include <algorithm>

#include "Tests.h"
#include "CommandLineHelper.h"
#include "ConsoleColor.h"

using namespace std;

inline bool exists_test(const String& name) {
	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

//------------------------------------------------------------------------------

void Test::ParseArguments(const Strings& arguments)
{
	std::cout << "arguments for this test are not yet parsed" << std::endl;
}

//------------------------------------------------------------------------------

void QualityTest::ParseArguments(const Strings& arguments)
{
	if (arguments.size() == 0)
	{
		return;
	}
	else if (arguments.size() == 1)
	{
		this->SnapshotSize = atoi(arguments[0].c_str());
	}
	else
	{
		throw "Can't parse the parameters for quality test the only argument is the snapshot size";
	}
}

//------------------------------------------------------------------------------

struct QualityResult
{
	int Sumation = 0;
	int count = 0;
};

void QualityTest::RunTest(String outputFilename)
{
	vector<QualityResult> results;

	Input* realInput = CreateInput(*find_if(this->descriptions.begin(), this->descriptions.end(), IsType<ComInput>));
	vector<StackInput*> stackInputs = CreateStackInputs(this->descriptions);
	vector<Algorithm*> algorithms = CreateAlgorithms(this->descriptions);

	Input* input = realInput;
	for (StackInput* stackinput : stackInputs)
	{
		stackinput->SetInternalInput(input);
		input = stackinput;
	}
	CaptureStackInput* capture = new CaptureStackInput();
	capture->SetInternalInput(input);
	input = capture;

	SplitStackInput* split = new SplitStackInput();
	split->SetInternalInput(input);
	split->Open();
	
	for (int i = 0; i < algorithms.size(); i++)
	{
		split->SetOutput(algorithms[i]);
		results.push_back(QualityResult());
	}
		
	int updates = 0;

	cout << "Start quality test..." << endl;

	while (!split->IsEnd())
	{
		for (int i = 0; !split->IsEnd() && i < this->SnapshotSize; i++)
		{
			input->ExecuteNextUpdate();
			updates++;
		}

		Graph* g = capture->GetCompleteGraph();

		cout << updates << " updates done, current graph size: " << g->size() << ", continuing ..." << endl;

		for (int i = 0; i < algorithms.size(); i++)
		{
			int cutSize = 0;
			for (Edge edge : *g)
			{
				vertex c1 = algorithms[i]->FindClusterIndex(edge.v1);
				vertex c2 = algorithms[i]->FindClusterIndex(edge.v2);
				if (c1 != c2)
					cutSize++;
			}

			results[i].Sumation += cutSize;
			results[i].count += 1;
		}
	}

	split->Close();

	cout << "Done testing, writing results..." << endl;

	bool exist = exists_test(outputFilename);

	//writes results
	ofstream output;
	if (exist)
		output.open(outputFilename, ios::out | ios::ate | ios::app);
	else
		output.open(outputFilename, ios::out);
	for (int i = 0; i < algorithms.size(); i++)
		output << static_cast<float>(results[i].Sumation) / static_cast<float>(results[i].count) << ",";
	output << endl;
	output.close();

	cout << "Finished writing" << endl;
}

//------------------------------------------------------------------------------

CaptureStackInput::CaptureStackInput()
{
	this->_currentGraph = new Graph();
}

//------------------------------------------------------------------------------

CaptureStackInput::~CaptureStackInput()
{
	delete this->_currentGraph;
}

//------------------------------------------------------------------------------

void CaptureStackInput::Add(Edge e)
{
	this->output->Add(e);
	this->_currentGraph->insert(e);
}

//------------------------------------------------------------------------------

Graph* CaptureStackInput::GetCompleteGraph()
{
	return this->_currentGraph;
}

//------------------------------------------------------------------------------

void CaptureStackInput::Remove(Edge e)
{
	this->output->Remove(e);
	Graph::iterator it = find(this->_currentGraph->begin(), this->_currentGraph->end(), e);
	if (it != this->_currentGraph->end())
		this->_currentGraph->erase(it);
}

//------------------------------------------------------------------------------
void SplitStackInput::SetOutput(Output* output)
{
	this->outputs.push_back(output);
}

void SplitStackInput::Open()
{
	if (this->is_open == false)
	{
		this->is_open = true;
		this->input->Open();
	}
}

void SplitStackInput::Close()
{
	if (this->is_open == true)
	{
		this->is_open = false;
		this->input->Close();
	}
}

void SplitStackInput::ExecuteNextUpdate()
{
	this->input->ExecuteNextUpdate();
}

bool SplitStackInput::IsEnd()
{
	return this->input->IsEnd();
}

void SplitStackInput::Add(Edge e)
{
	for (Output* o : this->outputs)
		o->Add(e);
}

void SplitStackInput::Remove(Edge e)
{
	for (Output* o : this->outputs)
		o->Remove(e);
}

void ThroughputTest::ParseArguments(const Strings& arguments)
{
	if (arguments.size() == 2)
	{
		this->queries = atoi(arguments[0].c_str());
		this->updates = atoi(arguments[1].c_str());
	}
	else
	{
		throw "Can't parse the parameters for throughput test, arguments are <queries> <updates>";
	}
}

void ThroughputTest::RunTest(String outputFilename)
{
	cout << "Start performance test..." << endl;

	Input* realInput = CreateInput(*find_if(this->descriptions.begin(), this->descriptions.end(), IsType<ComInput>));
	vector<StackInput*> stackInputs = CreateStackInputs(this->descriptions);
	Algorithm* algorithm = CreateAlgorithm(*find_if(this->descriptions.begin(), this->descriptions.end(), IsType<ComAlgorithm>));

	Input* input = realInput;
	for (StackInput* stackinput : stackInputs)
	{
		stackinput->SetInternalInput(input);
		input = stackinput;
	}
	input->SetOutput(algorithm);
	input->Open();
	
	int totalUpdates = 0;
	
	bool NoClusterWarning = false;
	bool EmptyClusterWarning = false;
	clock_t start = clock();
	while (!input->IsEnd())
	{
		for (int i = 0; !input->IsEnd() && i < this->updates; i++)
		{
			input->ExecuteNextUpdate();
			totalUpdates++;
			if (totalUpdates%10000 == 0)
				cout << totalUpdates << endl;
		}

		for (int i = 0; !input->IsEnd() && i < this->queries; i++)
		{
			int clusterCount = algorithm->CountClusters();
			if (clusterCount != 0)
			{
				int c1i = rand() % clusterCount;
				int c2i = rand() % clusterCount;

				Vertices c1 = algorithm->GetCluster(c1i);
				Vertices c2 = algorithm->GetCluster(c2i);

				if (c1.size() > 0 && c2.size() > 0)
				{
					int v1 = rand() % c1.size();
					int v2 = rand() % c2.size();

					algorithm->FindClusterIndex(v1);
					algorithm->FindClusterIndex(v2);

					//check if v1c and v2c are the same, but unneccary because performance test
				}
				else
				{
					EmptyClusterWarning = true;
				}
			}
			else
			{
				NoClusterWarning = true;
			}
		}
	}
	clock_t end = clock();

	input->Close();

	cout << "Done testing, writing results..." << endl;

	bool exist = exists_test(outputFilename);

	//writes results
	ofstream output;
	if (exist)
		output.open(outputFilename, ios::out | ios::ate | ios::app);
	else
		output.open(outputFilename, ios::out);
	
	clock_t  diff = end-start;
	if (diff != 0)
	{
		float diffSec = static_cast<float>(diff) / CLOCKS_PER_SEC;
		float throughput = updates/diffSec;
		output << throughput;
	}
	else
	{
		output << "-1";
	}
	
	output.close();

	cout << "Finished writing" << endl;

	if (NoClusterWarning)
	{
		cout << yellow << "Warning: There was a situation with no clusters" << white << endl;
	}
	if (EmptyClusterWarning)
	{
		cout << yellow << "Warning: There were empty clusters" << white << endl;
	}

}