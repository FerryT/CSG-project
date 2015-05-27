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
#include <algorithm>
#include <CommandLineHelper.h>

void OutputVisualization::RunTest(string outputFilename)
{
	this->input = CreateInput(*find_if(this->descriptions.begin(), this->descriptions.end(), IsType<ComInput>));

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
		fprintf(output, "\t%lu -- %lu;\n", edge.v1, edge.v2);
		vertices.insert(edge.v1);
		vertices.insert(edge.v2);
	}

	for (vertex v : vertices)
	{
		fprintf(output, "\t%lu [label=\"\",shape=point];\n", v);
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
	this->input = CreateInput(*find_if(this->descriptions.begin(), this->descriptions.end(), IsType<ComInput>));
	this->algorithm = CreateAlgorithm(*find_if(this->descriptions.begin(), this->descriptions.end(), IsType<ComAlgorithm>));
	this->stackinputs = CreateStackInputs(this->descriptions);

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
	capture->SetInternalInput(this->input);
	Input* input = capture;
	for (StackInput* stackinput : this->stackinputs)
	{
		stackinput->SetInternalInput(input);
		input = stackinput;
	}

	CaptureStackInput* capture2 = new CaptureStackInput();
	capture2->SetInternalInput(input);
	capture2->SetOutput(this->algorithm);
	capture2->Open();

	int updates = 0;

	for (int i = 0; !input->IsEnd() && (i < this->runTillUpdate || this->runTillUpdate == -1); i++)
	{
		input->ExecuteNextUpdate();
		updates++;
	}
	
	Graph* g = capture->GetCompleteGraph();
	Graph* g2 = capture2->GetCompleteGraph();

	set<vertex> vertices;

	for (Edge edge : *g)
	{
		vertex c1 = this->algorithm->FindClusterIndex(edge.v1);
		vertex c2 = this->algorithm->FindClusterIndex(edge.v2);

		bool isSampled = find_if(g2->begin(), g2->end(), [edge](Edge edge2){ return edge2 == edge; }) != g2->end();

		vertices.insert(edge.v1);
		vertices.insert(edge.v2);

		string color;
		string style;
		if (isSampled)
		{
			style = "solid";
		}
		else
		{
			style = "dashed";
		}
		if (c1 != c2)
		{
			color = "red";
		}
		else
		{
			color = "black";
		}

		fprintf(output, "\t%lu -- %lu [color=%s,style=%s];\n",
			edge.v1, edge.v2, color.c_str(), style.c_str());
	}
	
	for (int i = 0; i < this->algorithm->CountClusters(); i++)
	{
		fprintf(output, "\tsubgraph cluster%d {\n", i);
		vector<vertex> c = this->algorithm->GetCluster(i);
		for (vertex v : c)
		{
			fprintf(output, "\t\t%lu;\n", v);
		}
		fputs("\t}\n", output);
	}
	
	for (vertex v : vertices)
	{
		fprintf(output, "\t%lu [label=\"\",shape=point];\n", v);
	}
	
	capture->Close();
	fputs("}\n", output);

	if (CallDot)
		pclose(output);
	else
		fclose(output);
}
