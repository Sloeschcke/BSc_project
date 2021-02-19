#include <iostream>
#include "graph.h"
#include "hoved.h"
#include "pruner.h"
#include "sampler.h"
#include "tests.h"

using namespace std;

int main(int argc, char const *argv[])
{
    string graph_file = ".\\ReliablePaperImpl\\graph_file.inf";
	std::cout << "Hello ze world";
	Graph graph (7, graph_file);
	graph.readGraph();
	std::cout << "Hello ze world";
	testAll();
	return 0;
}
