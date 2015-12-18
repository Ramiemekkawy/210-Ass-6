#include <iostream>

#include "format.h"

#include "WGraph.h"

using namespace std;

int test1()
{
	WGraph G;
	G.load_from_disk(R"(D:\A2\GitHub\210-Ass-6\x64\Debug\CitiesG.txt)");
	G.print_graph();
	fmt::print( "\n\n" );
	G.print_edges();
	auto path = G.bf_trav();
	fmt::print( "\n\n" );
	for ( auto& step : path ) {
		fmt::print( "{} .{}\n", step.first, (char) (step.second + 'A') );
	}
	
	return 0;
}