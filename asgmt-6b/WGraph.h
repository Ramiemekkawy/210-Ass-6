// File: Graphs.h
/*
______________________________________________________________________________________________

  This file describes a structure for a weighted undirected graph with a maximum of vMax = 50
  vertices and eMax = vMax(vMax-1)/2 edges. The verices are numbered 0,1,...V-1. The graph is
  assumed to be on a text file in the form of an adjacency matrix. The weights on the edges are
  assumed to be positive integers with zero weight indicating the absence of an edge.
  An edge (u,v,w) existing between nodes (u) and (v) with weight (w) is modeled as a class
  (Edge). When loaded from the text file, the weights are stored in a  2-D
  array (AdjMatrix) representing the adjacency matrix. Another  array (edges) stores
  the non-zero edges in the graph. The functions declared here are needed to implement:

  (1) Breadth First Search (BFS) algorithm for graph traversal.
  (2) Dijkstra's Algorithm to compute the shortest paths from a given source node to all other
	  nodes in the graph.
_______________________________________________________________________________________________
*/

#ifndef GRAPHS_H
#define GRAPHS_H
#include <string>
#include "Edge.h"
using namespace std;

const int vMax = 50;					// Maximum number of vertices
const int eMax = vMax*(vMax - 1) / 2;		// Maximum number of edges

class WGraph {
public:
	WGraph();
	~WGraph();

// Public interface [unstable]
	char Vname( const int s ) const;		// Map vertex number to a name (character) 
	void load_from_disk( string fname );		// Get Graph from text File (fname)
	void print_graph() const;			// Display Adjacency Matrix
	int  n_vertices() const;		// Get number of vertices (V)
	int  n_edges() const;		// Get Number of Non-zero edges (E)
	void print_edges() const;			// Display Graph edges
	void bf_trav( int i = 1 );				// Breadth First Search Traversal (BFS) overloaded to traverse from given vertex
	void shortest_path( int s );			// Shortest paths from node (s)

//______________________________________________________________________________________________

private:
	int nVertices, nEdges;				// No.of vertices (V) and edges (E) 
	Weight adjMatrix[vMax][vMax];	// Adjacency Matrix
	Edge edges[eMax];			// Array of non-zero edges
	Weight distance[vMax];		// Distance array for shortest paths  
	int via[vMax];			// Via array for shortest paths  
	bool processed[vMax];		// processed array for shortest paths  
	void update_edges();			// Get Non-Zero edges from adjacency matrix  
	std::string WGraph::to_string( Edge e ) const;	// Output an edge (e)  
	void print_walk_path( int s, int i ) const;	// Print path from source (s) to destination (i)  
};
#endif // GRAPHS_H