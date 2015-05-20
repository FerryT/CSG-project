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