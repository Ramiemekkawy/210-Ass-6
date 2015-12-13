#include <iostream>
#include "Graphs.h"
using namespace std;

int main()
{
	Graphs G;
	G.getGraph("C:\Rami\Coding Library\210 Assignments\Assignment 6\Assignment 6\CitiesG.txt");
	G.dispGraph();
	G.dispEdges();
	G.BFS();
	system("pause");
}