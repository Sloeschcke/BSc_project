#include <iostream>
#include "graph.h"
#include "hoved.h"
#include "tests.h"
#include "Experiments.h"
#include <cstdio>

using namespace std;

int main(int argc, char const *argv[])
{
	string varyingK = "Varying K";
	string numNodes = "num_nodes";
	string edgeDegree = "edge_degree";

	std::cout << "Hello ze world \n";
	// testAll();
	runExperiments(true, varyingK);
	return 0;
}
	