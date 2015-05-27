#include <iostream>
#include <fstream>
#include <algorithm>

#include "../headers/Tests.h"
#include <CommandLineHelper.h>

using namespace std;

inline bool exists_test(const std::string& name) {
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

void QualityTest::ParseArguments(const vector<string>& arguments)
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

QualityTest::QualityTest()
{
}

//------------------------------------------------------------------------------

QualityTest::QualityTest(int snapshotSize)
{
	this->SnapshotSize = snapshotSize;
}

//------------------------------------------------------------------------------

struct QualityResult
{
	int Sumation = 0;
	int count = 0;
};

void QualityTest::RunTest(string outputFilename)
{
	vector<QualityResult> results;

	Input* realInput = CreateInput(*find_if(this->descriptions.begin(), this->descriptions.end(), IsType<ComInput>));
	vector<StackInput*> stackInputs = CreateStackInputs(this->descriptions);
	vector<Algorithm*> algorithms = CreateAlgorithms(this->descriptions);

	CaptureStackInput* capture = new CaptureStackInput();
	capture->SetInternalInput(realInput);
	Input* input = capture;
	for (StackInput* stackinput : stackInputs)
	{
		stackinput->SetInternalInput(input);
		input = stackinput;
	}
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

		cout << updates << " updates done, continuing ..." << endl;

		Graph* g = capture->GetCompleteGraph();

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
		output << static_cast<float>(results[i].Sumation) / static_cast<float>(results[i].count) << ";";
	output << endl;
	output.close();

	cout << "Done testing, writing results..." << endl;
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
	this->_currentGraph->push_back(e);
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