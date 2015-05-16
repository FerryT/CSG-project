#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using namespace std;

// a single vertex
typedef unsigned long vertex;

// and edge from and to vertex
struct edge {
	vertex v1;
	vertex v2;
	double p;

	edge() {}
	edge(vertex u, vertex v) : v1(u), v2(v), p(0.0) {}

	bool operator==(const edge& other) const
	{
		return v1 == other.v1 && v2 == other.v2;
	}
};

typedef vector<edge> Graph;

#endif
