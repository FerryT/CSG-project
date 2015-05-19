#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "GraphInput.h"
#include <queue>

struct TimedEdge {
	Edge e;
	int time;
};

class SlideWindow : public StackInput
{
	Input* input;
	int timesteps = 0;
	queue<TimedEdge> window;

public:
	int windowSize = 0;

	void ExecuteNextUpdate() override;
	bool IsEnd() override;

	void Add(const Edge& e) override;
	void Remove(const Edge& e) override;
};

class TumblingWindow : public StackInput
{
	Input* input;
	Graph window;
	bool removal = false;

public:
	int windowSize = 0;

	void ExecuteNextUpdate() override;
	bool IsEnd() override;

	void Add(const Edge& e) override;
	void Remove(const Edge& e) override;

};



#endif