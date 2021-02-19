#include "hoved.h"
#include "sampler.h"
#include "pruner.h"

void testPrune(){
	set<set<int>> vertices = {{1,2,3}, {1,2}, {1,4}};

	set<set<int>> expected = {{1,2,3}, {1,4}};
	assert(prune(vertices)==expected);
}

void testSampler(){
    string graph_file = "C:\\Users\\chris\\Documents\\6. Semester\\Bachelor Project\\BSc_project\\ReliablePaperImpl\\graph_file.inf";
	Graph graph (7, graph_file);
	graph.readGraph();
	vector<vector<vector<int>>> samples = sample(graph, 5);

	assert(samples.size() == 5);


}

void testAll(){
    testPrune();
	testSampler();
}

