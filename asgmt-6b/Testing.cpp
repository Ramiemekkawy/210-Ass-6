#include <iostream>
#include "WGraph.h"

using namespace std;

int test1()
{
	WGraph G;
	G.load_from_disk(R"(CitiesG.txt)");
	G.print_graph();
	G.print_edges();
	G.bf_trav();
	
	return 0;
}