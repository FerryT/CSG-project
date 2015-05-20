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


class ScaleFreeNetworkInput : public Input {
private:
	int nodes;
	float insertDeleteRatio;
	Graph CurrentGraph;
	vector<int>* NotInCurrentGraph;
	int maxUpdates;
	int currentUpdates;
	int* NodeDegree; 
	int totalDegree;
public:
	ScaleFreeNetworkInput(int nodes, float insertDeleteRatio, int maxUpdates);
	~ScaleFreeNetworkInput();

	void Open() override;
	void Close() override;
	void ExecuteNextUpdate() override;
	bool IsEnd() override;
};

#endif // RANDOMIZED_INPUT_H
