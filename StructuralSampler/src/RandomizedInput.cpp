#include <random>
#include <algorithm>
#include <time.h>

#include "RandomizedInput.h"


EulerNetworkInput::EulerNetworkInput(int nodes, float insertDeleteRatio, int maxUpdates)
{
	this->maxUpdates = maxUpdates;
	this->currentUpdates = 0;

	this->nodes = nodes;
	this->insertDeleteRatio = insertDeleteRatio;

	this->NotInCurrentGraph.reserve(nodes*(nodes / 2 - 1) + nodes / 2);
	this->CurrentGraph.reserve(nodes*(nodes / 2 - 1) + nodes / 2);

	for (int i = 0; i < nodes; i++)
	{
		for (int j = i+1; j < nodes; j++)
		{
			this->NotInCurrentGraph.push_back(Edge(i, j));
		}
	}
}

void EulerNetworkInput::Open()
{
}

void EulerNetworkInput::Close()
{
}

void EulerNetworkInput::ExecuteNextUpdate()
{
	if(this->CurrentGraph.empty() || rand() % 100 < insertDeleteRatio*100)
	{
		//inserts
		if (this->NotInCurrentGraph.size())
		{
			int item = rand() % this->NotInCurrentGraph.size();
			Edge e = this->NotInCurrentGraph[item];
			this->CurrentGraph.push_back(e);
			this->NotInCurrentGraph.erase(this->NotInCurrentGraph.begin()+item);
	
			this->output->Add(e);
		}
	}
	else
	{
		//deletes
		if (this->CurrentGraph.size())
		{
			int item = rand() % this->CurrentGraph.size();
			Edge e = this->CurrentGraph[item];
			this->NotInCurrentGraph.push_back(e);
			this->CurrentGraph.erase(this->CurrentGraph.begin() + item);
	
			this->output->Remove(e);
		}
	}

	this->currentUpdates++;
}

bool EulerNetworkInput::IsEnd()
{
	return this->currentUpdates >= this->maxUpdates;
}

void EulerNetworkInput::ParseArguments(const Strings &arguments)
{
	if (arguments.size() == 0)
	{
		return;
	}
	else if (arguments.size() == 3)
	{
		this->nodes = atoi(arguments[0].c_str());
		this->insertDeleteRatio = atof(arguments[1].c_str());
		this->maxUpdates = atoi(arguments[2].c_str());
	}
	else
	{
		throw "Can't parse the parameters for EulerNetworkInput, parameters are <nodes> <insert delete ratio> <updates>";
	}
}

BAModelInput::BAModelInput(int nodes)
{
	this->nodes = nodes;
	this->NodeDegree = new int[nodes];
	this->totalDegree = 0;
	this->currentSourceNode = 1;
	this->currentDestinationNode = 0;

	for (int i = 0; i < nodes; i++)
	{
		this->NodeDegree[i] = 0;
	}
}

BAModelInput::~BAModelInput()
{
	delete this->NodeDegree;
}

void BAModelInput::Open()
{
}

void BAModelInput::Close()
{
}

void BAModelInput::ExecuteNextUpdate()
{
	this->output->Add(Edge(this->currentSourceNode, this->currentDestinationNode));
	this->NodeDegree[this->currentSourceNode]++;
	this->NodeDegree[this->currentDestinationNode]++;
	this->totalDegree += 2;

	this->FindNextEdge();
}

void BAModelInput::FindNextEdge()
{
	while (true)
	{
		this->currentDestinationNode++;
		if (this->currentDestinationNode == this->currentSourceNode)
		{
			this->currentDestinationNode = 0;
			this->currentSourceNode++;
			if (this->currentSourceNode == this->nodes)
			{
				//end of input
				return;
			}
		}

		double sample = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
		double prob = static_cast<double>(this->NodeDegree[this->currentDestinationNode]) / static_cast<double>(this->totalDegree);
		if (sample < prob)
		{
			return;
		}
	}
}

bool BAModelInput::IsEnd()
{
	return this->currentSourceNode >= this->nodes;
}

void BAModelInput::ParseArguments(const Strings &arguments)
{
	if (arguments.size() == 0)
	{
		return;
	}
	else if (arguments.size() == 1)
	{
		this->nodes = atoi(arguments[0].c_str());

		delete this->NodeDegree;
		this->NodeDegree = new int[this->nodes];
		for (int i = 0; i < nodes; i++)
		{
			this->NodeDegree[i] = 0;
		}
	}
	else
	{
		throw "Can't parse the parameters for BAModelInput, parameters are <nodes>";
	}
}
