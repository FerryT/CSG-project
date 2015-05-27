#ifndef GRAPH_INPUT_H
#define GRAPH_INPUT_H

#include <string>

#include "Graph.h"
#include <iostream>

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
		Input() : output(0) {}
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
		virtual void ParseArguments(const vector<string>& arguments) { std::cout << "arguments for this input/stackinput are not yet parsed" << std::endl; }
};

class StackInput: public Input, public Output
{
protected:
	Input* input;
public:
	virtual void Add(Edge e) override;
	virtual void Remove(Edge e) override;
	virtual void Open() override;
	virtual void Close() override;
	virtual void ExecuteNextUpdate() override;
	virtual bool IsEnd() override;

	virtual void SetInternalInput(Input* input);
};




//------------------------------------------------------------------------------

class FileInput: public Input {
	public:
		std::string filename;
	
		FileInput(const char *fn) : Input(), filename(fn) {}
		FileInput() : Input() {}

		void ParseArguments(const vector<string>& arguments) override;

};

//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------

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

/*

//The classes that has to be implemented:

class DNSInput {
public:
	//Opens the input
	void Open();
	//Closes the input
	void Close();
	//this executed the next update, this can be a delete or a insert
	void ExecuteNextUpdate();
};

class PaperCitationsInput {
public:
	//Opens the input
	void Open();
	//Closes the input
	void Close();
	//this executed the next update, this can be a delete or a insert
	void ExecuteNextUpdate();
};

class WebGraphNotreDameInput {
public:
	//Opens the input
	void Open();
	//Closes the input
	void Close();
	//this executed the next update, this can be a delete or a insert
	void ExecuteNextUpdate();
};

class TwitterInput {
public:
	//Opens the input
	void Open();
	//Closes the input
	void Close();
	//this executed the next update, this can be a delete or a insert
	void ExecuteNextUpdate();
};

*/

//------------------------------------------------------------------------------

#endif // GRAPH_INPUT_H
