#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using namespace std;

// a single vertex
typedef int vertex;

// and edge from and to vertex
struct edge {
	vertex v1;
	vertex v2;
	float p;


	bool operator==(const edge& other) const
	{
		return v1 == other.v1 && v2 == other.v2;
	}
};

typedef vector<edge> graph;

class structuralReservoir{
private:
	graph g;
public:
	void insertEdge(edge e);//insert edge such that the graph is sorted by the p-values of the edges
	void removeEdge(edge e);//remove exactly this edge (equal including p-value, only remove 1 instance)
	edge removeLast();//remove the edge from g with the highest p-value, and return this edge
};

class supportReservoir{
private:
	graph g;
public:
	int lastGetEdgesLength = 0;//the length of the array returned with the last call to getEdges

	void insertEdge(edge e);//insert edge such that the graph is sorted by the p-values of the edges
	void removeEdge(edge e);//remove exactly this edge (equal including p-value, only remove 1 instance)
	edge * getEdges(float minP);//return all edges with p-values > minP, store the length of the array in lastGetEdgesLength
};

#endif