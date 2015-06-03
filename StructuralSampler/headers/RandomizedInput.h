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
	EulerNetworkInput(): EulerNetworkInput(100, 0.80f, 100) {};
	EulerNetworkInput(int nodes, float insertDeleteRatio, int maxUpdates);

	void Open() override;
	void Close() override;
	void ExecuteNextUpdate() override;
	bool IsEnd() override;

	void ParseArguments(const Strings &arguments) override;
};


class ScaleFreeNetworkInput : public Input {
private:
	int nodes;
	float insertDeleteRatio;
	Edges CurrentGraph;
	Vertices *NotInCurrentGraph;
	int maxUpdates;
	int currentUpdates;
	int* NodeDegree; 
	int totalDegree;
public:
	ScaleFreeNetworkInput() : ScaleFreeNetworkInput(100, 0.80f, 100) {};
	ScaleFreeNetworkInput(int nodes, float insertDeleteRatio, int maxUpdates);
	~ScaleFreeNetworkInput();

	void Open() override;
	void Close() override;
	void ExecuteNextUpdate() override;
	bool IsEnd() override;

	void ParseArguments(const Strings &arguments) override;
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
	BAModelInput() : BAModelInput(100) {};
	BAModelInput(int nodes);
	~BAModelInput();

	void Open() override;
	void Close() override;
	void ExecuteNextUpdate() override;
	bool IsEnd() override;

	void ParseArguments(const Strings &arguments) override;
};

#endif // RANDOMIZED_INPUT_H
