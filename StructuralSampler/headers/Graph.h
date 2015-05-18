#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using namespace std;

// a single vertex
typedef unsigned long vertex;

// and edge from and to vertex
struct Edge {
	vertex v1;
	vertex v2;
	double p;

	Edge() {}
	Edge(vertex u, vertex v) : v1(u), v2(v), p(0.0) {}

	bool operator==(const Edge& other) const
	{
		return v1 == other.v1 && v2 == other.v2;
	}

	bool operator!=(const Edge& other) const
	{
		return v1 != other.v1 || v2 != other.v2;
	}
};

typedef vector<Edge> Graph;

#endif
