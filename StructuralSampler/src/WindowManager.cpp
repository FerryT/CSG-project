#include "WindowManager.h"

void SlideWindow::Add(Edge e)
{
	TimedEdge timed_edge(e, timesteps);
	this->window.push(timed_edge);
	this->output->Add(e);
}

void SlideWindow::Remove(Edge e)
{
}

void SlideWindow::ExecuteNextUpdate()
{
	if (!this->window.empty() && this->timesteps - this->window.front().time > this->windowSize)
	{
		this->output->Remove(this->window.front().e);
		this->window.pop();
	}
	else
	{
		StackInput::ExecuteNextUpdate();
	}

	this->timesteps++;
}

bool SlideWindow::IsEnd()
{
	return this->input->IsEnd() && this->window.empty();
}


void TumblingWindow::ExecuteNextUpdate()
{
	if (window.size() >= this->windowSize)
	{
		removal = true;
	}
	else if (window.empty())
	{
		removal = false;
	}

	if (removal == true)
	{
		this->output->Remove(window.back());
		window.pop_back();
	}
	else
	{
		this->input->ExecuteNextUpdate();
	}
}

bool TumblingWindow::IsEnd()
{
	return StackInput::IsEnd() && window.empty();
}

void TumblingWindow::Add(Edge e)
{
	this->window.push_back(e);
	StackInput::ExecuteNextUpdate();
}

void TumblingWindow::Remove(Edge e)
{
}