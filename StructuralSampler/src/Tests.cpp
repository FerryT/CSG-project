#include <iostream>
#include <fstream>
#include <algorithm>

#include "../headers/Tests.h"

using namespace std;

//------------------------------------------------------------------------------

void QualityTest::RunTest(string outputFilename)
{
	ofstream output;

	output.open(outputFilename, ios::out);

	output << "updates,cutsize" << endl;

	CaptureStackInput* capture = new CaptureStackInput();
	capture->SetInternalInput(input);
	capture->SetAlgorithm(this->algorithm);

	int updates = 0;

	while (!this->input->IsEnd())
	{
		for (int i = 0; !this->input->IsEnd() && i < this->SnapshotSize; i++)
		{
			capture->ExecuteNextUpdate();
			updates++;
		}

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

	output.close();

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
