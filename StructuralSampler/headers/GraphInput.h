#ifndef GRAPH_INPUT_H
#define GRAPH_INPUT_H

#include "Graph.h"

//------------------------------------------------------------------------------

//An Output
class Output
{
	public:
		//adds an edge to the graph
		virtual void Add(Edge e) = 0;
		//removes an edge from the graph
		virtual void Remove(Edge e) = 0;
};

//An input 
class Input {
	protected:
		Output* output;

	public:
		Input() : output(NULL) {}
		//Opens the input
		virtual void Open() = 0;
		//Closes the input
		virtual void Close() = 0;
		//this executed the next update, this can be a delete or a insert
		virtual void ExecuteNextUpdate() = 0;
		//check if it is at the end of it's input
		virtual bool IsEnd() = 0;
		//sets the algorithm
		virtual void SetOutput(Output* output) { this->output = output; }

		//parses arguments for configuration
		virtual void ParseArguments(const Strings &arguments);
};

class StackInput: public Input, public Output
{
protected:
	Input* input;
public:
	virtual void Add(Edge e);
	virtual void Remove(Edge e);
	virtual void Open();
	virtual void Close();
	virtual void ExecuteNextUpdate();
	virtual bool IsEnd();

	virtual void SetInternalInput(Input* input);
};

//------------------------------------------------------------------------------

class FileInput: public Input {
	public:
		String filename;
	
		FileInput(const char *fn) : Input(), filename(fn) {}
		FileInput() : Input() {}

		void ParseArguments(const Strings &arguments);

};

// METIS graph file input
class MGraphFileInput: public FileInput {
	public:
		MGraphFileInput(const char *fn) : FileInput(fn), file(0) {}
		MGraphFileInput() : FileInput(), file(0) {}
	
		virtual void Open();
		virtual void Close();
		virtual void ExecuteNextUpdate();
		virtual bool IsEnd();

	private:
		struct File;
		File *file;
};

// edge list file input
class EdgeFileInput: public FileInput {
	public:
		EdgeFileInput(const char *fn) : FileInput(fn), file(0) {}
		EdgeFileInput() : FileInput(), file(0) {}

		virtual void Open();
		virtual void Close();
		virtual void ExecuteNextUpdate();
		virtual bool IsEnd();

	private:
		struct File;
		File *file;
};

//------------------------------------------------------------------------------

#endif // GRAPH_INPUT_H
