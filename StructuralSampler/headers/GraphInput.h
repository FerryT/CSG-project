#ifndef GRAPH_INPUT_H
#define GRAPH_INPUT_H

#include "Algorithm.h"

//An input 
class Input {
	public:
		//Opens the input
		virtual void Open() = 0;
		//Closes the input
		virtual void Close() = 0;
		//this executed the next update, this can be a delete or a insert
		virtual void ExecuteNextUpdate() = 0;
};

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

#endif