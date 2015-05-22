#include <iostream>
#include <fstream>
#include <algorithm>

#include "../headers/Tests.h"

using namespace std;

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

void QualityTest::RunTest(string outputFilename)
{
	ofstream output;

	output.open(outputFilename, ios::out);

	output << "updates,cutsize" << endl;

	CaptureStackInput* capture = new CaptureStackInput();
	capture->SetInternalInput(input);
	capture->SetAlgorithm(this->algorithm);
	capture->Open();

	int updates = 0;

	cout << "Start quality test..." << endl;

	while (!this->input->IsEnd())
	{
		for (int i = 0; !this->input->IsEnd() && i < this->SnapshotSize; i++)
		{
			capture->ExecuteNextUpdate();
			updates++;
		}

		cout << updates << " updates done, saving results..." << endl;

		Graph* g = capture->GetCompleteGraph();

		int cutSize = 0;
		for (Edge edge : *g)
		{
			vertex c1 = this->algorithm->FindClusterIndex(edge.v1);
			vertex c2 = this->algorithm->FindClusterIndex(edge.v2);
			if (c1 != c2)
				cutSize++;
		}

		output << updates << "," << cutSize << endl;
	}

	capture->Close();
	output.close();
	cout << "Done" << endl;

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
	this->algorithm->Add(e);
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
	this->algorithm->Remove(e);
	Graph::iterator it = find(this->_currentGraph->begin(), this->_currentGraph->end(), e);
	this->_currentGraph->erase(it);
}

//------------------------------------------------------------------------------
