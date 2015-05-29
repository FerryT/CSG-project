#ifndef GRAPH_H
#define GRAPH_H

#include <cstdlib>
#include <set>
#include <vector>

// a single vertex
typedef unsigned long vertex;

// a number refering to a cluster
typedef unsigned short clusterid;

// and edge from and to vertex
struct Edge {
	vertex v1;
	vertex v2;
	double p;

	Edge(vertex u, vertex v);
	Edge(vertex u, vertex v, double p) : v1(u < v ? u : v), v2(u < v ? v : u), p(p) {  }

	bool operator==(const Edge& other) const
	{
		return v1 == other.v1 && v2 == other.v2;
	}
	bool operator!=(const Edge& other) const
	{
		return v1 != other.v1 || v2 != other.v2;
	}
	bool operator <(const Edge &other)
	{
		if (v1 == other.v1)
			return v2 < other.v2;
		return v1 < other.v1;
	}
};

typedef std::set<Edge> Graph;
typedef std::vector<vertex> Vertices;

#endif
