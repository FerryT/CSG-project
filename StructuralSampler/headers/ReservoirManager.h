#ifndef RESERVOIR_MANAGER_H
#define RESERVOIR_MANAGER_H

#include "Graph.h"

//------------------------------------------------------------------------------

class Reservoir {
	protected:
		Graph graph;
	
	public:
		virtual void Add(const Edge &e) = 0;
		virtual void RemoveExact(const Edge &e) = 0;
};

//------------------------------------------------------------------------------

class StructuralReservoir : public Reservoir {
	public:
		//insert edge such that the graph is sorted by the p-values of the edges
		void Add(const Edge &e);
		//remove exactly this edge (equal including p-value, only remove 1 instance)
		void RemoveExact(const Edge &e);
		//remove this edge (equal excluding p-value, only remove 1 instance), return the p-value of the removed edge
		double Remove(const Edge &e);
		//remove the edge from g with the highest p-value, and return this edge
		Edge RemoveLast();
		//returns if g contains an edge equal to e (excluding p-value)
		bool HasEdge(const Edge &e);
};

//------------------------------------------------------------------------------

class SupportReservoir : public Reservoir {
	public:
		//insert edge such that the graph is sorted by the p-values of the edges
		void Add(const Edge &e);
		//remove exactly this edge (equal including p-value, only remove 1 instance)
		void RemoveExact(const Edge &e);
		//try to remove an edge equal to e excluding the p-value (only 1 instance), return true if edge was present and false otherwise
		bool Remove(const Edge &e);
		//return all edges with p-values > minP, store the length of the array in lastGetEdgesLength
		vector<Edge> GetEdges(double minP);
};

//------------------------------------------------------------------------------

#endif // RESERVOIR_MANAGER_H
