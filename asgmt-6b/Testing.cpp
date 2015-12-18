#include <iostream>

#include "format.h"

#include "WGraph.h"

using namespace std;

int test1()
{
	WGraph G;
	
	G.load_from_disk(R"(D:\A2\GitHub\210-Ass-6\data-files\TestG.txt)");
	
	G.print_graph();
	
	fmt::print( "\n\n" );
	G.print_edges();
	
	auto path = G.bf_trav();
	fmt::print( "\n\n" );
	for ( auto& step : path ) {
		fmt::print( "{} .{}\n", step.first, (char) (step.second + 'A') );
	}

	//G.print_walk_path( 0, 5 );
	//G.print_walk_path( 3, 7 );
	//G.print_walk_path( 1, 9 );
	//G.print_walk_path( 6, 2 );
	G.print_walk_path( 0, 0 );
	G.print_walk_path( 0, 4 );
	G.print_walk_path( 0, 2 );
	G.print_walk_path( 0, 3 );
	G.print_walk_path( 0, 6 );
	G.print_walk_path( 5, 2 );
	G.print_walk_path( 3, 1 );
	
	return 0;
}