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

	output.close();
}

