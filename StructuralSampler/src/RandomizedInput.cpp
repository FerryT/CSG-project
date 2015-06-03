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
		int item = rand() % this->NotInCurrentGraph.size();
		Edge e = this->NotInCurrentGraph[item];
		this->CurrentGraph.push_back(e);
		this->NotInCurrentGraph.erase(this->NotInCurrentGraph.begin()+item);

		this->output->Add(e);
	}
	else
	{
		//deletes
		int item = rand() % this->CurrentGraph.size();
		Edge e = this->CurrentGraph[item];
		this->NotInCurrentGraph.push_back(e);
		this->CurrentGraph.erase(this->CurrentGraph.begin() + item);

		this->output->Remove(e);
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

ScaleFreeNetworkInput::ScaleFreeNetworkInput(int nodes, float insertDeleteRatio, int maxUpdates)
{
	this->maxUpdates = maxUpdates;
	this->currentUpdates = 0;

	this->nodes = nodes;
	this->insertDeleteRatio = insertDeleteRatio;

	this->CurrentGraph.reserve(nodes*nodes);
	this->NotInCurrentGraph = new Vertices[nodes];
	this->NodeDegree = new int[nodes];

	for (int i = 0; i < nodes; i++)
	{
		this->NotInCurrentGraph[i].reserve(nodes);
		for (int j = 0; j < nodes; j++)
		{
			if (j != i)
				this->NotInCurrentGraph[i].push_back(j);
		}
		this->NodeDegree[i] = 1;
	}
	this->totalDegree = nodes*1;
}

ScaleFreeNetworkInput::~ScaleFreeNetworkInput()
{
	delete this->NodeDegree;
}

void ScaleFreeNetworkInput::Open()
{
}

void ScaleFreeNetworkInput::Close()
{
}

void ScaleFreeNetworkInput::ExecuteNextUpdate()
{
	if (this->CurrentGraph.empty() || rand() % 100 < insertDeleteRatio * 100)
	{
		//inserts
		int item = rand() % this->totalDegree;
		int tmp = 0;
		int from = -1;
		for (int i = 0; i < this->nodes; i++)
		{
			if (tmp + this->NodeDegree[i] > item)
			{
				from = i;
				break;
			}
			else
			{
				tmp += this->NodeDegree[i];
			}
		}

		item = rand() % this->NotInCurrentGraph[from].size();
		int to = this->NotInCurrentGraph[from][item];

		this->NotInCurrentGraph[from].erase(find(this->NotInCurrentGraph[from].begin(), this->NotInCurrentGraph[from].end(), to));
		this->NotInCurrentGraph[to].erase(find(this->NotInCurrentGraph[to].begin(), this->NotInCurrentGraph[to].end(), from));
		
		this->NodeDegree[from]++;
		this->NodeDegree[to]++;
		this->totalDegree += 2;

		Edge e = Edge(from, to);
		this->CurrentGraph.push_back(e);

		this->output->Add(e);
	}
	else
	{
		//deletes
		int item = rand() % this->CurrentGraph.size();
		Edge e = this->CurrentGraph[item];
		this->NotInCurrentGraph[e.v1].push_back(e.v2);
		this->NotInCurrentGraph[e.v2].push_back(e.v1);
		this->CurrentGraph.erase(this->CurrentGraph.begin() + item);

		this->NodeDegree[e.v1]--;
		this->NodeDegree[e.v2]--;
		this->totalDegree -= 2;

		this->output->Remove(e);
	}

	this->currentUpdates++;
}

bool ScaleFreeNetworkInput::IsEnd()
{
	return this->currentUpdates >= this->maxUpdates;
}

void ScaleFreeNetworkInput::ParseArguments(const Strings &arguments)
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
		throw "Can't parse the parameters for ScaleFreeNetworkInput, parameters are <nodes> <insert delete ratio> <updates>";
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