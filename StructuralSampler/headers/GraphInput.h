#ifndef GRAPH_INPUT_H
#define GRAPH_INPUT_H

#include <string>

#include "Algorithm.h"

//------------------------------------------------------------------------------

//An Output
class Output
{
	//adds an edge to the graph
	virtual void Add(const Edge &e) = 0;
	//removes an edge from the graph
	virtual void Remove(const Edge &e) = 0;
};

//An input 
class Input {
	protected:
		Output* algorithm;

	public:
		Input() : algorithm(0) {}
		//Opens the input
		virtual void Open() = 0;
		//Closes the input
		virtual void Close() = 0;
		//this executed the next update, this can be a delete or a insert
		virtual void ExecuteNextUpdate() = 0;
		//check if it is at the end of it's input
		virtual bool IsEnd() = 0;
		//sets the algorithm
		void SetAlgorithm(Output* alg) { algorithm = alg; }
};




//------------------------------------------------------------------------------

class FileInput: public Input {
	public:
		const std::string filename;
	
		FileInput(const char *fn) : Input(), filename(fn) {}
};

//------------------------------------------------------------------------------

// METIS graph file input
class MGraphFileInput: public FileInput {
	public:
		MGraphFileInput(const char *fn) : FileInput(fn), file(0) {}
	
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

class EulerNetworkInput {
public:
	//Opens the input
	void Open();
	//Closes the input
	void Close();
	//this executed the next update, this can be a delete or a insert
	void ExecuteNextUpdate();
};

class ScaleFreeNetworkInput {
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
