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
	EulerNetworkInput(): EulerNetworkInput(100, 0.80f, 100) {};
	EulerNetworkInput(int nodes, float insertDeleteRatio, int maxUpdates);

	void Open() override;
	void Close() override;
	void ExecuteNextUpdate() override;
	bool IsEnd() override;

	void ParseArguments(const vector<string>& arguments) override;
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
	ScaleFreeNetworkInput() : ScaleFreeNetworkInput(100, 0.80f, 100) {};
	ScaleFreeNetworkInput(int nodes, float insertDeleteRatio, int maxUpdates);
	~ScaleFreeNetworkInput();

	void Open() override;
	void Close() override;
	void ExecuteNextUpdate() override;
	bool IsEnd() override;

	void ParseArguments(const vector<string>& arguments) override;
};

#endif // RANDOMIZED_INPUT_H
