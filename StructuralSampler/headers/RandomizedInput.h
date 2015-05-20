#ifndef RANDOMIZED_INPUT_H
#define RANDOMIZED_INPUT_H

#include "Graph.h"
#include "GraphInput.h"

class EulerNetworkInput: public Input {
private:
	int nodes;
	float insertDeleteRatio;
	Graph CurrentGraph;
	Graph NotInCurrentGraph;
	int maxUpdates;
	int currentUpdates;
public:
	EulerNetworkInput(int nodes, float insertDeleteRatio, int maxUpdates);

	void Open() override;
	void Close() override;
	void ExecuteNextUpdate() override;
	bool IsEnd() override;
};

/*

class EulerNetworkInput {
public:
//Opens the input
void Open();
//Closes the input
void Close();
//this executed the next update, this can be a delete or a insert
void ExecuteNextUpdate();
};

class ScaleFreeNetworkInput {
public:
//Opens the input
void Open();
//Closes the input
void Close();
//this executed the next update, this can be a delete or a insert
void ExecuteNextUpdate();
};

*/

#endif // RANDOMIZED_INPUT_H
