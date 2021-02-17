#include <iostream>
#include "graph.h"
#include "hoved.h"
using namespace std;
int main(int argc, char const *argv[])
{
    string graph_file = "graph_file.inf";
	std::cout << "Hello ze world";
	Graph graph (7, graph_file);
	graph.readGraph();
	std::cout << "Hello ze world";
	
	return 0;
}