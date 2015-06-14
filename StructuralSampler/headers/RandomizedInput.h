#ifndef RANDOMIZED_INPUT_H
#define RANDOMIZED_INPUT_H

#include "Graph.h"
#include "GraphInput.h"

class EulerNetworkInput: public Input {
private:
	int nodes;
	float insertDeleteRatio;
	Edges CurrentGraph;
	Edges NotInCurrentGraph;
	int maxUpdates;
	int currentUpdates;
public:
	EulerNetworkInput(int nodes = 100, float insertDeleteRatio = 0.8f, int maxUpdates = 100);

	void Open();
	void Close();
	void ExecuteNextUpdate();
	bool IsEnd();

	void ParseArguments(const Strings &arguments);
};

class BAModelInput : public Input {
private:
	int nodes;
	int* NodeDegree;
	int totalDegree;
	int currentSourceNode;
	int currentDestinationNode;

	void FindNextEdge();
public:
	BAModelInput(int nodes = 100);
	~BAModelInput();

	void Open();
	void Close();
	void ExecuteNextUpdate();
	bool IsEnd();

	void ParseArguments(const Strings &arguments);
};

#endif // RANDOMIZED_INPUT_H
