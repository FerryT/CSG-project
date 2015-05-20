#include "RandomizedInput.h"
#include <random>
#include <time.h>

EulerNetworkInput::EulerNetworkInput(int nodes, float insertDeleteRatio, int maxUpdates)
{
	srand(time(NULL));

	this->maxUpdates = maxUpdates;
	this->currentUpdates = 0;

	this->nodes = nodes;
	this->insertDeleteRatio = insertDeleteRatio;

	this->NotInCurrentGraph.reserve(nodes*nodes);
	this->CurrentGraph.reserve(nodes*nodes);

	for (int i = 0; i < nodes; i++)
	{
		for (int j = 0; j < nodes; j++)
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

		this->algorithm->Add(e);
	}
	else
	{
		//deletes
		int item = rand() % this->CurrentGraph.size();
		Edge e = this->CurrentGraph[item];
		this->NotInCurrentGraph.push_back(e);
		this->CurrentGraph.erase(this->CurrentGraph.begin() + item);

		this->algorithm->Remove(e);
	}

	this->currentUpdates++;
}

bool EulerNetworkInput::IsEnd()
{
	return this->currentUpdates >= this->maxUpdates;
}

ScaleFreeNetworkInput::ScaleFreeNetworkInput(int nodes, float insertDeleteRatio, int maxUpdates)
{
	srand(time(NULL));

	this->maxUpdates = maxUpdates;
	this->currentUpdates = 0;

	this->nodes = nodes;
	this->insertDeleteRatio = insertDeleteRatio;

	this->CurrentGraph.reserve(nodes*nodes);
	this->NotInCurrentGraph = new vector<int>[nodes];
	this->NodeDegree = new int[nodes];

	for (int i = 0; i < nodes; i++)
	{
		this->NotInCurrentGraph[i].reserve(nodes);
		for (int j = 0; j < nodes; j++)
		{
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

		this->algorithm->Add(e);
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

		this->algorithm->Remove(e);
	}

	this->currentUpdates++;
}

bool ScaleFreeNetworkInput::IsEnd()
{
	return this->currentUpdates >= this->maxUpdates;
}