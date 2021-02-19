#include "hoved.h"
#include "sampler.h"
#include "pruner.h"

void testPrune(){
	set<set<int>> vertices = {{1,2,3}, {1,2}, {1,4}};

	set<set<int>> expected = {{1,2,3}, {1,4}};
	assert(prune(vertices)==expected);
}

void testSampler(){
    string graph_file = ".\\ReliablePaperImpl\\graph_file.inf";
	Graph graph (7, graph_file);
	graph.readGraph();
	vector<vector<vector<int>>> samples = sample(graph, 5);

	assert(samples.size() == 5);
	for (int i = 0; i < samples.size(); i++){
		vector<vector<int>> world = samples[i];
		assert(world.size()==graph.adjList.size());
		for (int j = 0; j < samples[i].size(); j++){
			vector<int> worldVertex = world[j];
			vector<int> graphVertex = graph.adjList[j];
			assert(worldVertex.size() <= graphVertex.size());
		}
	}
	


}

void testAll(){
    testPrune();
	testSampler();
}

