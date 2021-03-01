#include "hoved.h"
#include "utility.h"
#include "apriori.h"
#include "peeling.h"
#include "fastPeeling.h"

using namespace std;


void testPrune(){
	set<set<int>> vertices = {{1},{1,2}, {1,2,3}, {1,4},{2,3,5},{2,3}};
	set<set<int>> pruned = prune(vertices);

	set<set<int>> expected = {{1,2,3}, {1,4},{2,3,5}};
	assert(pruned==expected);
}

void testConnectedComponnets(){
		string graph_file = ".\\ReliablePaperImpl\\graph_file2.inf";
		//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file2.inf";
		int numEdges = 4;
		int numNodes = 6;
		Graph graph (numNodes,numEdges, graph_file);
		graph.readGraph();
		vector<vector<vector<int>>> samples = sample(graph, 3);

	    std::cout << "Started testConnectedComponnets: \n";
		vector<vector<int>> cC = connectedComponents(&samples);
    }

void testConnectedComponnets2(){
		string graph_file = ".\\ReliablePaperImpl\\graph_file_certain.inf";
		//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file_certain.inf";
		int numEdges = 11;
		int numNodes = 11;
		Graph graph (numNodes,numEdges, graph_file);
		graph.readGraph();
		vector<vector<vector<int>>> samples = sample(graph, 1);

	    std::cout << "Started testConnectedComponnets: \n";

		vector<vector<int>> cC = connectedComponents(&samples);
		vector<vector<int>> expected = {{0, 1, 4, 3, 2, 5, 6}, {7,8}, {9,10}};

		assert(cC ==  expected);
    }

void testSampler(){
    string graph_file = ".\\ReliablePaperImpl\\graph_file.inf";
    //string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file.inf";
	Graph graph (7,10, graph_file);
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

void testApriori(){
	// string graph_file = ".\\ReliablePaperImpl\\graph_file_certain.inf";
	// //string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file_certain.inf";
	// int numEdges = 11;
	// int numNodes = 11;
	// Graph graph (numNodes,numEdges, graph_file);
	// graph.readGraph();
	// vector<vector<vector<int>>> samples = sample(graph, 1);

	// std::cout << "Started testConnectedComponnets: \n";

	// vector<vector<int>> cC = connectedComponents(&samples);
	vector<vector<int>> transactions = {
		{1,3,4}, {2,3,5}, {1,2,3,5}, {2,5}
	};
	set<set<int>> maximalFI = getMFI(transactions, 0.5, 4);
	set<set<int>> expected = {{2,3,5},{1,3}};
	//assert(maximalFI == expected);
}

void testReliability(){
	string graph_file = ".\\ReliablePaperImpl\\graph_file_certain.inf";
	//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file_certain.inf";
	int numEdges = 11;
	int numNodes = 11;
	Graph graph (numNodes,numEdges, graph_file);
	graph.readGraph();
	vector<vector<vector<int>>> samples = sample(graph, 2);
	set<int> subgraph = {0,1};

	double reliability = subgraphReliability(samples, subgraph);
	assert(reliability == 1);
}

//TODO test subgraph DFS
void DFS(){
	
}


void testPeeling(){
	set<set<int>> res = runPeeling(".\\ReliablePaperImpl\\graph_file_certain.inf", 11, 11, 1, 0.5);
	set<set<int>> expected = {{0, 1, 4, 3, 2, 5, 6}, {7,8}, {9,10}};
	assert(res == expected);
}

void testFastPeeling(){
	set<set<int>> res = runFastPeeling(".\\ReliablePaperImpl\\graph_file_certain.inf", 11, 11, 1, 0.5);
	set<set<int>> expected = {{0, 1, 4, 3, 2, 5, 6}, {7,8}, {9,10}};
	assert(res == expected);
}

void testSetSorting(){
	set<set<int>> unSorted = {{1,2},  {6,7,8},{3,4,5}};
	set<int> tofind = {1,2,3};
	int count = unSorted.count(tofind);
	set<set<int>, customCompareLength> sorted;
	copy(unSorted.begin(), unSorted.end(), inserter(sorted, sorted.begin()));
	assert(sorted == sorted);
}
//TODO FIX result
void testPeelingNonDeterministic(){
	set<set<int>> res = runPeeling(".\\ReliablePaperImpl\\graph_file.inf", 7, 10, 100, 0.5);
	set<set<int>> expected = {{0, 1, 4, 3, 2, 5, 6}, {7,8}, {9,10}};
	assert(res == expected);
}

void testAll(){
    // testPrune();
	// testApriori();
	// testConnectedComponnets();
	// testConnectedComponnets2();
	// testReliability();
	// testSampler();
	// testPeeling();
	// testPeelingNonDeterministic();
	testSetSorting();
	testFastPeeling();
}

