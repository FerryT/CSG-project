#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "GraphInput.h"
#include <queue>

struct TimedEdge {
	Edge e;
	int time;
};

class SlideWindow: Input, Output
{
	Input* input;
	int timesteps = 0;
	queue<TimedEdge> window;

public:
	int windowSize = 0;

	void Open() override;
	void Close() override;
	void ExecuteNextUpdate() override;
	bool IsEnd() override;

	void Add(const Edge& e) override;
	void Remove(const Edge& e) override;

	void SetInternalInput(Input* input);
};



#endif