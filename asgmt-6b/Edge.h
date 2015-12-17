// File: Edge.h
/* _______________________________________________________________
An edge (u,v,w) existing between nodes (u) and (v) with weight (w)
is modeled as a class (Edge).
__________________________________________________________________
*/

#ifndef EDGE_H
#define EDGE_H

typedef unsigned int Weight;		// weights are positive integers

class Edge {
public:
	int u;
	int v;
	Weight w;
	bool operator< ( const Edge &e );
	bool operator<= ( const Edge &e );
};
#endif // EDGE_H

