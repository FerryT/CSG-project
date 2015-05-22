#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <set>

#include "Visualize.h"
#include "Tests.h"

#ifdef _MSC_VER
#	define popen _popen
#	define pclose _pclose
#endif

void OutputVisualization::RunTest(string outputFilename)
{
	FILE *output;

	if (CallDot)
	{
		string dotCall = "dot -Tpng -Kneato -o" + outputFilename;
		output = popen(dotCall.c_str(), "wt");
	}
	else
	{
		output = fopen(outputFilename.c_str(), "wt");
	}
	
	if (!output)
		throw "Unable to open stream while visualizing.";

	fputs("graph G {\n\tsplines = false;\n", output);

	NullOutput* nullOutput = new NullOutput();
	CaptureStackInput* capture = new CaptureStackInput();
	capture->SetInternalInput(input);
	capture->SetOutput(nullOutput);
	capture->Open();

	while (!this->input->IsEnd())
	{
		capture->ExecuteNextUpdate();
	}

	Graph* g = capture->GetCompleteGraph();

	set<vertex> vertices;

	for (Edge edge : *g)
	{
		fprintf(output, "\t%ul -- %ul;\n", edge.v1, edge.v2);
		vertices.insert(edge.v1);
		vertices.insert(edge.v2);
	}

	for (vertex v : vertices)
	{
		fprintf(output, "\t%ul [label=\"\",shape=point];\n", v);
	}

	fputs("}\n", output);

	capture->Close();
	if (CallDot)
		pclose(output);
	else
		fclose(output);
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
	FILE *output;

	if (CallDot)
	{
		string dotCall = "dot -Tpng -o" + outputFilename;
		output = popen(dotCall.c_str(), "wt");
	}
	else
	{
		output = fopen(outputFilename.c_str(), "wt");
	}
	
	if (!output)
		throw "Unable to open stream while visualizing.";

	fputs("graph G {\n\tsplines = false;\n", output);

	CaptureStackInput* capture = new CaptureStackInput();
	capture->SetInternalInput(input);
	capture->SetOutput(this->algorithm);
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

		fprintf(output, "\t%ul -- %ul %s;\n",
			edge.v1, edge.v2, c1 != c2 ? "[color=red]" : "");
	}

	for (int i = 0; i < this->algorithm->CountClusters(); i++)
	{
		fprintf(output, "\tsubgraph cluster %d {\n", i);
		vector<vertex> c = this->algorithm->GetCluster(i);
		for (vertex v : c)
		{
			fprintf(output, "\t\t%ul;\n", v);
		}
		fputs("\t}\n", output);
	}

	for (vertex v : vertices)
	{
		fprintf(output, "\t%ul [label=\"\",shape=point];\n", v);
	}
	
	capture->Close();
	fputs("}\n", output);
	if (CallDot)
		pclose(output);
	else
		fclose(output);
}
