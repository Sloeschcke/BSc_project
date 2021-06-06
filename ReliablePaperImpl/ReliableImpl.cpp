#include <iostream>
#include "Graph.h"
#include "Hoved.h"
#include "Tests.h"
#include "Experiments.h"
#include <cstdio>

using namespace std;

int main(int argc, char const *argv[])
{
	std::cout << "Hello ze world \n";
	string path = "C:\\Users\\chris\\Documents\\6. Semester\\Bachelor Project\\BSc_project\\GraphsGeneration\\processed_graphs\\Varying_K\\0\\1.txt";
	// ValueTime valTime = run2StepExperiment(path, 2, 0.05, 0.01);
	// testAll();
	// Precision2Step();
	// runNumNodesFDist();
	// ToyDataSetValidation();
	run1StepAll();
	// allNumNodesRunExperiments();
	// allVaryingKRunExperiments();
	// allEdgeDegreeRunExperiments();
	// runAllNaiveExperiments();
	return 0;
}	