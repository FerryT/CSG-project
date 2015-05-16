#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include "Graph.h"

class GraphManager {
	private:
		graph g;
	
	public:
		//just insert the edge into the graph
		void Add(const edge &e);
		//remove exactly this edge (equal including p-value, only remove 1 instance)
		void RemoveExact(const edge &e);
		//remove this edge (equal excluding p-value, only remove 1 instance)
		void Remove(const edge &e);
		//if the 'constraint' mentioned in the paper is satisfied
		bool ConstraintSatisfied();
};

#endif
