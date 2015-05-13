#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using namespace std;

// a single vertex
typedef int vertex;

// and edge from and to vertex
struct edge {   // Declare PERSON struct type
	vertex v1;
	vertex v2;
	float p;
};

typedef vector<edge> graph;

#endif