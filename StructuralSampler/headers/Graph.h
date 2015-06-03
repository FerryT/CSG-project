#ifndef GRAPH_H
#define GRAPH_H

#include <cstdlib>
#include <set>
#include <vector>
#include <string>

// a single vertex
typedef unsigned long vertex;

// a number refering to a cluster
typedef unsigned short clusterid;
const clusterid unclustered = (clusterid) -1;

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
	bool operator <(const Edge &other) const
	{
		if (v1 == other.v1)
			return v2 < other.v2;
		return v1 < other.v1;
	}
	struct minP
	{
		bool operator() (const Edge &e1, const Edge &e2) const
		{
			if (e1.p == e2.p) return e1 < e2; return e1.p < e2.p;
		}
	};
	struct less2 // Swapped lexicographic order
	{
		bool operator() (const Edge &e1, const Edge &e2) const
		{
			if (e1.v2 == e2.v2) return e1.v1 < e2.v1; return e1.v2 < e2.v2;
		}
	};
};

typedef std::set<Edge> Graph;
typedef std::set<Edge, Edge::less2> Less2Graph;
typedef std::set<Edge, Edge::minP> MinPGraph;
typedef std::vector<Edge> Edges;
typedef std::vector<vertex> Vertices;

typedef std::string String;
typedef std::vector<String> Strings;

#endif
