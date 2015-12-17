// FILE: graphs.cpp
// Graph Library Implementation File

#include <fstream>		// For reading file from disk
#include <iostream>
#include <string>		// For file name
#include <iomanip>
#include <queue>    // For BFS
#include <stack>
#include <tuple>

#include "Edge.h"	// Deinition of an Edge
#include "WGraph.h"
#include "priority-queue.hh"

using namespace std;

// Constructor
WGraph::WGraph()
	: nEdges(), nVertices()
{}

// Destructor
WGraph::~WGraph()
{}


// Map vertex number (0,1,2,..) to (A,B,C,..)
char WGraph::Vname( const int s ) const
{
	char Vertex;
	return Vertex = (char) (s + (int)'A');
}

// Get Graph from text File (file name is stored in string fname)
// Graph is stored in adjacency matrix
void WGraph::load_from_disk( string fname )
{
   // Local data ...
	Weight wi;
	ifstream source( fname );

	if ( source.fail() ) {
		cout << "Error reading file" << endl;
		return;
	}
	source >> nVertices;	// Read no. of verices (V)

	// Input weights from the file into a 2-D 
	// Adjacency Matrix of V rows and V columns
	while ( !source.eof() ) {
		for ( int r = 0; r < nVertices; r++ ) {
			for ( int s = 0; s < nVertices; s++ ) {
				source >> wi; // get V weights from columns in file
				adjMatrix[r][s] = wi; 	// put V weights in adjacency matrix at row [r] column [s]
			}
		}
	}

	source.close();// close file
	update_edges();
}

// Display Adjacency Matrix
void WGraph::print_graph() const
{
	int i, j;
	cout << "Adjacency Matrix\n";
	for ( i = 0; i < nVertices; i++ ) {
		for ( j = 0; j < nVertices; j++ )
			cout << setw( 3 ) << adjMatrix[i][j] << " ";
		cout << endl;
	}
}

// Get Non-Zero edges from adjacency matrix
// and store them in array edges[]. 
void WGraph::update_edges()
{
	int r, c;
	int i = 0;
	Weight weight;

	// Only examine weights above the diagonal 
	for ( r = 0; r <= nVertices - 2; r++ )
		for ( c = r + 1; c <= nVertices - 1; c++ ) {
			weight = adjMatrix[r][c];
			if ( weight > 0 ) {
				// save (r,c,weight) in edges[i]
				edges[i].u = r;
				edges[i].v = c;
				edges[i].w = weight;
				i++;
			}
		}

	nEdges = i;		// Number of non-zero edges

}

// Get number of vertices (V)	
int WGraph::n_vertices() const
{
	return nVertices;
}

// Get Number of Non-zero edges (E)
int WGraph::n_edges() const
{
	return nEdges;
}

// Output an edge (e): Vertex names and weight
std::string WGraph::to_string( Edge e ) const
{
	return Vname( e.u ) + (std::string)" " + Vname( e.v ) + " " + std::to_string( e.w ) + "\n";
}

// Display Graph Edges
void WGraph::print_edges() const
{
	cout << "Graph Edges\n";
	for ( int i = 0; i < nEdges; i++ )
		cout << to_string( edges[i] );
}

// Shortest paths from node s
// uses Dijkstra's Algorithm
void WGraph::shortest_path( int s )
{
	stdx::BiHeap2<Weight, Weight> q;

	this->distance[s] = 0;

	for ( Weight v = 0; v < nVertices; ++v ) {
		this->distance[v] = (unsigned) -1;
		via[v] = -1;
		q.push( { distance[v], v } );
	}

	this->distance[s] = 0;

	while ( !q.empty() ) {
		auto u = q.pull().unwrap();

		// for neighbors
		for ( auto nb = 0; nb < this->nVertices; ++nb ) {
			auto alt = distance[u.data] + adjMatrix[u.data][nb];
			if ( alt < distance[nb] ) {
				distance[nb] = alt;
				via[nb] = u.data;

				q.set_priority( u.data, alt );
			}
		}
	}

	// `distance[]` and `via[]` have been set to the result.
}

// Print path (vertex names) from source (s) to destination (i)
void WGraph::print_walk_path( int source, int target ) const
{
	auto target_p = std::find( std::begin( via ), std::end( via ), target ) - std::begin( via );

	std::stack<char> st;

	while ( target_p != -1 ) {
		st.push( Vname( target_p ) );
		target_p = via[target_p];
	}

	while ( !st.empty() ) {
		fmt::print( ". {}\n", st.top() );
		st.pop();
	}
}

// Breadth First Search Traversal overload that outputs traversal as queue pops
void WGraph::bf_trav( int i )
{
	int trav_hist[vMax];
	int order = 0;
	std::queue<Weight> Q;
	int
		t;
	int const
		unseen = -1,
		hold = -2;

	for ( int k = 1; k <= nVertices; k++ )
		trav_hist[k] = unseen;

	Q.push( i );
	while ( !Q.empty() ) {
		i = Q.front();

		Q.pop();
		trav_hist[i] = ++order;
		for ( t = 0; t < nVertices; t++ )  // Scan from left to right
			if ( adjMatrix[i][t] != 0 ) {
				if ( trav_hist[t] == unseen ) {
					Q.push( t ); trav_hist[t] = hold;
				}
			}
	}
}