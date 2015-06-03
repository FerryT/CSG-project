#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "GraphInput.h"
#include <queue>

struct TimedEdge {
	Edge e;
	int time;
	TimedEdge(const Edge &edge, int t = 0) : e(edge), time(t) {}
};

class SlideWindow : public StackInput
{
	Input* input;
	int timesteps = 0;
	std::queue<TimedEdge> window;

public:
	int windowSize = 0;

	void ExecuteNextUpdate() override;
	bool IsEnd() override;

	void Add(Edge e) override;
	void Remove(Edge e) override;
};

class TumblingWindow : public StackInput
{
	Input* input;
	Edges window;
	bool removal = false;

public:
	int windowSize = 0;

	void ExecuteNextUpdate() override;
	bool IsEnd() override;

	void Add(Edge e) override;
	void Remove(Edge e) override;

};



#endif