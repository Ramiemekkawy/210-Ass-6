// FILE: graphs.cpp
// Graph Library Implementation File

#include <fstream>		// For reading file from disk
#include <iostream>
#include <string>		// For file name
#include <iomanip>
#include "Edge.h"	// Deinition of an Edge
#include <queue>    // For BFS


using namespace std;

// Constructor
Graphs::Graphs()
{
	E = V = order = 0;
}

// Destructor
Graphs::~Graphs()
{
}


// Map vertex number (0,1,2,..) to (A,B,C,..)
char Graphs::Vname(const int s) const
{
	char Vertex;
	return Vertex = s + 65;
}

// Get Graph from text File (file name is stored in string fname)
// Graph is stored in adjacency matrix
void Graphs::getGraph(string fname)		
{
   // Local data ...
	weightType wi;
	ifstream source;

	source.open(fname.c_str());
	if (source.fail())
	{
		cout << "Error reading file"<<endl;
		return;
	}
	source >> V;	// Read no. of verices (V)
	
	// Input weights from the file into a 2-D 
	// Adjacency Matrix of V rows and V columns
	while (!source.eof())
	{
		for (int r = 0; r < V; r++)
		{
			for (int s = 0; s < V; s++) 
			{
				source >> wi; // get V weights from columns in file
				AdjMatrix[r][s] = wi; 	// put V weights in adjacency matrix at row [r] column [s]
			}
		}
	}

	source.close();// close file
	getEdges();
}

// Display Adjacency Matrix
void Graphs::dispGraph() const
{
	int i,j;
	cout<<"Adjacency Matrix\n";
	for(i=0; i<V; i++)
	{
		for(j=0; j<V; j++)
			cout<< setw(3)<< AdjMatrix[i][j] << " ";
		cout<< endl;
	}
}

// Get Non-Zero edges from adjacency matrix
// and store them in array edges[]. 
void Graphs::getEdges()			
{
	int r , c;
	int i = 0;
	weightType weight;

	// Only examine weights above the diagonal 
	for (r = 0; r <= V-2; r++)
		for (c = r+1; c <= V-1; c++)
		{
			weight = AdjMatrix[r][c];
			if (weight > 0)
			{
				// save (r,c,weight) in edges[i]
				edges[i].u = r;
				edges[i].v = c;
				edges[i].w = weight;
				i++;
			}
		}

	E = i;		// Number of non-zero edges
	
}

// Get number of vertices (V)	
int Graphs::No_of_Verices() const 				
{
	return V;
}

// Get Number of Non-zero edges (E)
int Graphs::No_of_Edges() const 					
{
	return E;
}

// Output an edge (e): Vertex names and weight
void Graphs::printEdge(Edge e) const 			
{
	cout << Vname(e.u) << " " << Vname(e.v) << " " << e.w << endl;
}

// Display Graph Edges
void Graphs::dispEdges() const
{
	cout<<"Graph Edges\n";
	for (int i = 0; i < E; i++) 
		printEdge(edges[i]);
}

// Shortest paths from node s
// uses Dijkstra's Algorithm
void Graphs::shPath(int s)
{
	// Implement the function here
}

// Print path (vertex names) from source (s) to destination (i)
void Graphs::printPath(int s, int i) const
{
	// Implement the function here
}

// Breadth First Search Traversal that outputs traversal as queue pops
void Graphs::BFS()									
{ 
	int i = 1;
	queue <weightType> Q;
	int t, unseen = -2, hold = -1;
	order = 0;

	for (int k = 1; k <= V; k++)
		val[k] = unseen;

	cout << "Breadth First Search Traversal: " << endl;
	Q.push(i);
	while (!Q.empty())
	{
		i = Q.front();
		cout << i << endl;
		Q.pop();
		val[i] = ++order;
		for (t = 0; t < V; t++)  // Scan from left to right
			if (AdjMatrix[i][t] != 0)
			{
				if (val[t] == unseen)
				{
					Q.push(t); val[t] = hold;
				}
			}
	}
}

// Breadth First Search Traversal overload that outputs traversal as queue pops
void Graphs::BFS(int i)
{
	queue <weightType> Q;
	int t, unseen = -2, hold = -1;
	
	for (int k = 1; k <= V; k++) 
		val[k] = unseen;

	cout << "Breadth First Search Traversal: " << endl;
	Q.push(i);
	while (!Q.empty())
	{
		i = Q.front();
		cout << i << endl;
		Q.pop();
		val[i] = ++order;
		for (t = 0; t < V; t++)  // Scan from left to right
			if (AdjMatrix[i][t] != 0)
			{
				if (val[t] == unseen)
				{
					Q.push(t); val[t] = hold;
				}
			}
	}
}