#ifndef RESERVOIR_MANAGER_H
#define RESERVOIR_MANAGER_H

#include "Graph.h"

//------------------------------------------------------------------------------

class Reservoir {
	protected:
		graph g;
	
	public:
		virtual void Add(const edge &e) = 0;
		virtual void RemoveExact(const edge &e) = 0;
};

//------------------------------------------------------------------------------

class StructuralReservoir : public Reservoir {
	private:
		graph g;
	
	public:
		//insert edge such that the graph is sorted by the p-values of the edges
		void Add(const edge &e);
		//remove exactly this edge (equal including p-value, only remove 1 instance)
		void RemoveExact(const edge &e);
		//remove this edge (equal excluding p-value, only remove 1 instance), return the p-value of the removed edge
		double Remove(const edge &e);
		//remove the edge from g with the highest p-value, and return this edge
		edge RemoveLast();
		//returns if g contains an edge equal to e (excluding p-value)
		bool HasEdge(const edge &e);
};

//------------------------------------------------------------------------------

class SupportReservoir : public Reservoir {
	private:
		graph g;
	
	public:
		//the length of the array returned with the last call to getEdges
		int lastGetEdgesLength = 0;
	
		//insert edge such that the graph is sorted by the p-values of the edges
		void Add(const edge &e);
		//remove exactly this edge (equal including p-value, only remove 1 instance)
		void RemoveExact(const edge &e);
		//try to remove an edge equal to e excluding the p-value (only 1 instance), return true if edge was present and false otherwise
		bool Remove(const edge &e);
		//return all edges with p-values > minP, store the length of the array in lastGetEdgesLength
		edge *GetEdges(double minP);
};

//------------------------------------------------------------------------------

#endif // RESERVOIR_MANAGER_H
