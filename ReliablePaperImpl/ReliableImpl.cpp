#include <iostream>
#include "graph.h"
#include "hoved.h"
#include "tests.h"
#include "Experiments.h"
#include <cstdio>

using namespace std;

int main(int argc, char const *argv[])
{
	std::cout << "Hello ze world \n";
	// string path = "C:\\Users\\chris\\Documents\\6. Semester\\Bachelor Project\\BSc_project\\GraphsGeneration\\processed_graphs\\Varying_K\\0\\1.txt";
	// ValueTime valTime = run2StepExperiment(path, 2, 0.05, 0.01);
	// testAll();
	// Precision2Step();
	// runNumNodesFDist();
	// ToyDataSetValidation();
	// long double eps = 0.05;
    // long double delta = 0.01;
    // int k = 3;
	// runExperiments(true, "2Step", "num_nodes", eps, delta, k);
	// allNumNodesRunExperiments();
	allEdgeDegreeRunExperiments();
	// allVaryingKRunExperiments();
	// runAllNaiveExperiments();
	// runAllDeltaExperiments();
	// runAllEpsilonExperiments();
	return 0;
}	
