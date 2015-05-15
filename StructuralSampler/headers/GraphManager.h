#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include "Graph.h"

class graphManager{
public:
	vector<edge> theGraph;
	void insertEdge(edge e);//just insert the edge into the graph
	void removeEdgeExact(edge e);//remove exactly this edge (equal including p-value, only remove 1 instance)
	void removeEdge(edge e);//remove this edge (equal excluding p-value, only remove 1 instance)
	bool constraintSatisfied();//if the 'constraint' mentioned in the paper is satisfied
};

#endif