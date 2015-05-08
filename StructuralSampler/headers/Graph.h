#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using namespace std;

// a single vertex
using vertex = int;

// and edge from and to vertex
struct edge {   // Declare PERSON struct type
	vertex v1;
	vertex v2;
};

using graph = vector<edge>;

#endif