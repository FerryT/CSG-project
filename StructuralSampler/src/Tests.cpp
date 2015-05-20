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

	QualityTestCapture* capture = new QualityTestCapture();
	capture->RealAlgorithm = this->algorithm;

	this->input->SetAlgorithm(capture);

	int updates = 0;

	while (!this->input->IsEnd())
	{
		for (int i = 0; !this->input->IsEnd() && i < this->SnapshotSize; i++)
		{
			this->input->ExecuteNextUpdate();
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

QualityTestCapture::QualityTestCapture()
{
	this->_currentGraph = new Graph();
}

//------------------------------------------------------------------------------

QualityTestCapture::~QualityTestCapture()
{
	delete this->_currentGraph;
}

//------------------------------------------------------------------------------

void QualityTestCapture::Add(Edge e)
{
	this->RealAlgorithm->Add(e);
	this->_currentGraph->push_back(e);
}

//------------------------------------------------------------------------------

int QualityTestCapture::FindClusterIndex(vertex u)
{
	return this->RealAlgorithm->FindClusterIndex(u);
}

//------------------------------------------------------------------------------

vector<vertex> QualityTestCapture::FindCluster(vertex u)
{
	return this->RealAlgorithm->FindCluster(u);
}

//------------------------------------------------------------------------------

int QualityTestCapture::CountClusters()
{
	return this->RealAlgorithm->CountClusters();
}

//------------------------------------------------------------------------------

Graph* QualityTestCapture::GetCompleteGraph()
{
	return this->_currentGraph;
}

//------------------------------------------------------------------------------

void QualityTestCapture::Remove(Edge e)
{
	this->RealAlgorithm->Remove(e);
	Graph::iterator it = find(this->_currentGraph->begin(), this->_currentGraph->end(), e);
	this->_currentGraph->erase(it);
}

//------------------------------------------------------------------------------
