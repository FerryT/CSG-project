#include "WindowManager.h"

void SlideWindow::Add(const Edge& e)
{
	TimedEdge timed_edge;
	timed_edge.e = e;
	timed_edge.time = this->timesteps;
	this->window.push(timed_edge);
	this->algorithm->Add(e);
}

void SlideWindow::Remove(const Edge& e)
{
}

void SlideWindow::Open()
{
	this->input->Open();
}

void SlideWindow::Close()
{
	this->input->Close();
}

void SlideWindow::ExecuteNextUpdate()
{
	if (!this->window.empty() && this->timesteps - this->window.front().time > this->windowSize)
	{
		this->algorithm->Remove(this->window.front().e);
		this->window.pop();
	}
	else
	{
		this->input->ExecuteNextUpdate();
	}

	this->timesteps++;
}

bool SlideWindow::IsEnd()
{
	return this->input->IsEnd() && this->window.empty();
}

void SlideWindow::SetInternalInput(Input* input)
{
	this->input = input;
	input->SetAlgorithm(this);
}