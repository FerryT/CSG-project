#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <set>

#include "Visualize.h"
#include "Tests.h"

void OutputVisualization::RunTest(string outputFilename)
{
	ofstream output;

	if (CallDot)
	{
		string dotCall = "dot -Tpng -Kneato -o" + outputFilename;
		FILE* dot = _popen(dotCall.c_str(), "wt");
		output = ofstream(dot);
	}
	else
	{
		output.open(outputFilename, ios::out);
	}

	output << "graph G {" << endl;

	output << "splines = false;" << endl;

	NullOutput* nullOutput = new NullOutput();
	CaptureStackInput* capture = new CaptureStackInput();
	capture->SetInternalInput(input);
	capture->SetAlgorithm(nullOutput);
	capture->Open();

	while (!this->input->IsEnd())
	{
		capture->ExecuteNextUpdate();
	}

	Graph* g = capture->GetCompleteGraph();

	set<vertex> vertices;

	for (Edge edge : *g)
	{
		output << edge.v1 << " -- " << edge.v2 << ";" << endl;
		vertices.insert(edge.v1);
		vertices.insert(edge.v2);
	}

	for (vertex v : vertices)
	{
		output << v << " [label=\"\",shape=point];" << endl;
	}

	output << "}" << endl;

	capture->Close();
	output.close();
}

void VisualizeResults::ParseArguments(const vector<string>& arguments)
{
	if (arguments.size() == 0)
	{
		return;
	}
	else if (arguments.size() == 2)
	{
		this->runTillUpdate = atoi(arguments[0].c_str());
		this->CallDot = arguments[1] == "1";
	}
	else
	{
		throw "Can't parse the parameters for quality test, parameters are <updates> <0|1 for a dot call>";
	}
}

VisualizeResults::VisualizeResults(int runTillUpdate)
{
	this->runTillUpdate = runTillUpdate;
}

void VisualizeResults::RunTest(string outputFilename)
{
	ofstream output;

	if (CallDot)
	{
		string dotCall = "dot -Tpng -o" + outputFilename;
		FILE* dot = _popen(dotCall.c_str(), "wt");
		output = ofstream(dot);
	}
	else
	{
		output.open(outputFilename, ios::out);
	}

	output << "graph G {" << endl;
	output << "splines = false;" << endl;

	CaptureStackInput* capture = new CaptureStackInput();
	capture->SetInternalInput(input);
	capture->SetAlgorithm(this->algorithm);
	capture->Open();

	int updates = 0;

	for (int i = 0; !this->input->IsEnd() && (i < this->runTillUpdate || this->runTillUpdate == -1); i++)
	{
		capture->ExecuteNextUpdate();
		updates++;
	}

	Graph* g = capture->GetCompleteGraph();

	set<vertex> vertices;

	for (Edge edge : *g)
	{
		vertex c1 = this->algorithm->FindClusterIndex(edge.v1);
		vertex c2 = this->algorithm->FindClusterIndex(edge.v2);

		vertices.insert(edge.v1);
		vertices.insert(edge.v2);

		if (c1 != c2)
			output << edge.v1 << " -- " << edge.v2 << "[color=red];" << endl;
		else
			output << edge.v1 << " -- " << edge.v2 << ";" << endl;
	}

	for (int i = 0; i < this->algorithm->CountClusters(); i++)
	{
		output << "subgraph cluster" << i << " {" << endl;
		vector<vertex> c = this->algorithm->GetCluster(i);
		for (vertex v : c)
		{
			output << v << ";" << endl;
		}
		output << "}" << endl;
	}

	for (vertex v : vertices)
	{
		output << v << " [label=\"\",shape=point];" << endl;
	}
	
	capture->Close();
	output << "}" << endl;
	output.close();
}