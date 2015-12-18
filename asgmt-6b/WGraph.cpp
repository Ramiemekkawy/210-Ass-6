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
char WGraph::v_name( Weight s )
{
	char Vertex;
	return Vertex = (char) (s + (int)'A');
}

// Get Graph from text File (file name is stored in string fname)
// Graph is stored in adjacency matrix
void WGraph::load_from_disk( string fname )
{
	// Local data ...
	ifstream source( fname );

	if ( source.fail() ) {
		cout << "Error reading file" << endl;
		return;
	}
	else
		cout << "File loaded successfully." << endl;

	source >> nVertices;	// Read no. of verices (V)
	adjMatrix.resize( nVertices );
	for ( auto& a : adjMatrix )
		a.resize( nVertices );

	distance.resize( nVertices );
	prev.resize( nVertices );

	// Input weights from the file into a 2-D 
	// Adjacency Matrix of V rows and V columns
	for ( size_t r = 0; r < nVertices; r++ ) {
		for ( size_t s = 0; s < nVertices; s++ ) {
			if ( !source.fail() && !source.eof() ) {
				// get V weights from columns in file
				source >> adjMatrix[r][s]; 	// put V weights in adjacency matrix at row [r] column [s]
			}
			else
				break;
		}
	}

	update_edges();
}

// Display Adjacency Matrix
void WGraph::print_graph() const
{
	if ( nVertices < 1 ) {
		fmt::print( "\n<!>Fatal Error! Not enough time to study... Also, your graph is fun-sized (0 or less)." );
		return;
	}

	Weight i, j;

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
	Weight
		r,
		c,
		weight;

	edges.clear();

	// Only examine weights above the diagonal 
	for ( r = 0; r < nVertices - 1; r++ )
		for ( c = r + 1; c < nVertices; c++ ) {
			weight = adjMatrix[r][c];
			if ( weight > 0 ) {
				edges.emplace_back( r, c, weight );
			}
		}

	nEdges = edges.size();		// Number of non-zero edges
}

// Get number of vertices (V)	
auto WGraph::n_vertices() const -> Weight
{
	return nVertices;
}

// Get Number of Non-zero edges (E)
auto WGraph::n_edges() const -> Weight
{
	return nEdges;
}

// Output an edge (e): Vertex names and weight
std::string WGraph::to_string( Edge e ) const
{
	return (std::string)"(" + v_name( e.u ) + ", " + v_name( e.v ) + ", " + std::to_string( e.w ) + ")\n";
}

// Display Graph Edges
void WGraph::print_edges() const
{
	cout << "Graph Edges:\n";
	for ( size_t i = 0; i < nEdges; i++ )
		cout << to_string( edges[i] );
}

// Shortest paths from node s
// uses Dijkstra's Algorithm
void WGraph::calc_shortest_paths( int source )
{
	// the core of the algorithm; our self-sorting/bubbling set of vertices
	stdx::PriorityQueue<Weight, Weight> q;

	// populate the set 
	for ( Weight v = 0; v < nVertices; ++v ) {
		// direct distance.
		this->distance[v] = adjMatrix[source][v];
		if ( distance[v] == 0 )
			// infinite distance
			distance[v] = (unsigned) -1;
		// default previous or 'via' step.
		prev[v] = source;
		// push vertex 'v' with weight from source 's' = 'distance[v]'
		q.push( { distance[v], v } );
	}

	// distance from source to source = 0
	this->distance[source] = 0;

	// reset priority/weight of 's'/source to 0.
	q.set_priority( source, 0 );

	while ( !q.empty() ) {
		// pull one vertex and work on neighbors
		auto u = q.pull().unwrap();

		// for each neighbor
		for ( size_t nb = 0; nb < this->nVertices; ++nb ) {
			// check direct distance from source 's' to neighbor 'nb' vs distance
			// from 's' to current vertex ('u') + distance between
			// 'u' and 'nb'.
			auto alt = distance[u.data] + adjMatrix[u.data][nb];
			if ( alt < distance[nb] ) {
				// update shortest paths from 's' to 'nb' to the new shortest path
				// via 'u'.
				distance[nb] = alt;
				prev[nb] = u.data;

				q.set_priority( u.data, alt );
			}
		}
	}

	// `distance[]` and `via[]` have been set to the result.
}

// Print path (vertex names) from source (s) to destination (i)
void WGraph::print_walk_path( int source, int target )
{
	//auto target_p = std::find( std::begin( prev ), std::end( prev ), target ) - std::begin( prev );

	if ( source < 0 || source >= nVertices
		 || target < 0 || target >= nVertices )
		fmt::print( "\nError! Input source '{}'"
					" and target '{}' invalid for this graph.\n",
					source,
					target );

	this->calc_shortest_paths( source );

	/**\
	cout << "\n[";
	for ( auto& s : prev ) {
		fmt::print( "{}, ", s );
	}
	cout << "]\n";
	/**/

	std::stack<char> st;

 	while ( target != source ) {
		st.push( v_name( target ) );
		target = prev[target];
	} 

	fmt::print( "==> {}, ", v_name( source ) );
	while ( !st.empty() ) {
		fmt::print( "{}, ", st.top() );
		st.pop();
	}
	cout << endl;
}

// Breadth First Search Traversal overload that outputs traversal as queue pops
std::vector<std::pair<int, int>> WGraph::bf_trav( int i )
{
	std::vector<int> trav_hist( nVertices );
	int order = 0;
	std::queue<Weight> Q;
	int const
		unseen = -1,
		hold = -2;

	for ( size_t k = 0; k < nVertices; k++ )
		trav_hist[k] = unseen;

	Q.push( i );
	while ( !Q.empty() ) {
		i = Q.front();
		Q.pop();

		trav_hist[i] = ++order;

		// push all the neighbors
		for ( size_t t = 0; t < nVertices; t++ ) {  // Scan from left to right
			if ( adjMatrix[i][t] != 0 ) {
				if ( trav_hist[t] == unseen ) {
					Q.push( t );
					trav_hist[t] = hold;
				}
			}
		}
	}

	std::vector<std::pair<int, int>> rv;
	for ( int i = 0; i < trav_hist.size(); ++i ) {
		if ( trav_hist[i] >= 0 )
			rv.emplace_back( trav_hist[i], i );
	}

	stdx::heap_sort_ip( &rv, std::less<std::pair<int, int>>() );

	return rv;
}