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
	void removeEdgeExact(edge e);//remove exactly this edge (equal including p-value, only remove 1 instance)
	float removeEdge(edge e);//remove this edge (equal excluding p-value, only remove 1 instance), return the p-value of the removed edge
	edge removeLast();//remove the edge from g with the highest p-value, and return this edge
	bool hasEdge(edge e);//returns if g contains an edge equal to e (excluding p-value)
};

class supportReservoir{
private:
	graph g;
public:
	int lastGetEdgesLength = 0;//the length of the array returned with the last call to getEdges

	void insertEdge(edge e);//insert edge such that the graph is sorted by the p-values of the edges
	void removeEdgeExact(edge e);//remove exactly this edge (equal including p-value, only remove 1 instance)
	bool removeEdge(edge e);//try to remove an edge equal to e excluding the p-value (only 1 instance), return true if edge was present and false otherwise
	edge * getEdges(float minP);//return all edges with p-values > minP, store the length of the array in lastGetEdgesLength
};

#endif