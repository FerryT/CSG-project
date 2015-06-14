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
private:
	int timesteps;
	std::queue<TimedEdge> window;

public:
	int windowSize;

	SlideWindow() : timesteps(0), windowSize(0) {}

	void ExecuteNextUpdate();
	bool IsEnd();

	void Add(Edge e);
	void Remove(Edge e);

	void ParseArguments(const Strings& arguments);
};

class TumblingWindow : public StackInput
{
	Edges window;
	bool removal = false;

public:
	int windowSize = 0;

	TumblingWindow() : removal(false), windowSize(0) {}

	void ExecuteNextUpdate();
	bool IsEnd();

	void Add(Edge e);
	void Remove(Edge e);

	void ParseArguments(const Strings& arguments);
};



#endif