#include <cstdlib>

#include "Graph.h"

//------------------------------------------------------------------------------

Edge::Edge(vertex u, vertex v) : v1(u), v2(v)
{
	if (u < v)
		{ v1 = u; v2 = v; }
	else
		{ v1 = v; v2 = u; }
	p = (double) (rand()) / (double) (RAND_MAX);
}

//------------------------------------------------------------------------------
