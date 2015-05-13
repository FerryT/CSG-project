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

#endif