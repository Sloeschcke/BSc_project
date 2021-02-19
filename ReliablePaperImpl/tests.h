#include "hoved.h"
#include "sampler.h"
#include "pruner.h"
#include "utility.h"

void testPrune(){
	set<set<int>> vertices = {{1,2,3}, {1,2}, {1,4}};

	set<set<int>> expected = {{1,2,3}, {1,4}};
	assert(prune(vertices)==expected);
}

void testConnectedComponnets(){
		string graph_file = ".\\ReliablePaperImpl\\graph_file.inf";
		//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file2.inf";
		int numEdges = 4;
		int numNodes = 6;
		Graph graph (numNodes,numEdges, graph_file);
		graph.readGraph();
		vector<vector<vector<int>>> samples = sample(graph, 1);

	    std::cout << "Started testConnectedComponnets: \n";
		set<set<int>> cC = connectedComponents(&samples[0]);
    }

void testConnectedComponnets2(){
		// string graph_file = ".\\ReliablePaperImpl\\graph_file2.inf";
		string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file_certain.inf";
		int numEdges = 11;
		int numNodes = 11;
		Graph graph (numNodes,numEdges, graph_file);
		graph.readGraph();
		vector<vector<vector<int>>> samples = sample(graph, 1);

	    std::cout << "Started testConnectedComponnets: \n";
		set<set<int>> cC = connectedComponents(&samples[0]);
    }

void testSampler(){
    string graph_file = ".\\ReliablePaperImpl\\graph_file.inf";
    //string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file.inf";
	Graph graph (7,9, graph_file);
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
	  std::cout << "Sampling test finished: \n";
}

void testAll(){
    testPrune();
	testConnectedComponnets();
	testSampler();
}

