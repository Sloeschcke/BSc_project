#include "hoved.h"
#include "utility.h"
#include "apriori.h"
#include "peeling.h"
#include "fastPeeling.h"
#include "TopKPeeling.h"
#include "IterApriori.h"
#include "naiveTopKPeeling.h"
using namespace std;


void printMFCS(vector<Candidate> MFCS){
	for (auto elem: MFCS)
	{
		for(auto elem2: elem.nodes){
			cout << elem2 << ",";
		}
		cout << "\n";
	}
}

void printMFCSWhichAreNodesAndReliability(set<NodesAndReliability> MFCS){
	for(auto elem: MFCS){
		for(auto elem2: elem.nodes){
			cout << elem2 << ",";
		}
		cout << "\n";
	}
}

string abspath = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt";
// string abspath = "/Users/sebastianloeschcke/Desktop/6.semester/BSc";
// string abspath = "C:\\Users\\chris\\Documents\\6. Semester\\Bachelor Project";
void testPrune(){
	vector<vector<int>> vertices = {{1},{1,2}, {1,2,3}, {1,4},{2,3,5},{2,3}};
	vector<vector<int>> pruned = pruneVector(vertices);

	vector<vector<int>> expected = {{1,2,3}, {1,4},{2,3,5}};
	assert(pruned==expected);
}

void testPruneMatrix(){
	vector<vector<vector<int>>> vertices = {{{1}, {2}, {4}, {10}}, {{1,2}, {1,4}, {2,10}}, {{1,2,4}}};
	vector<vector<int>> pruned = pruneMatrix(vertices);

	vector<vector<int>> expected = {{2,10}, {1,2,4}};
	assert(pruned==expected);
}

void testConnectedComponnets(){
		string graph_file = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file2.inf";
		//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file2.inf";
		int numEdges = 4;
		int numNodes = 6;
		Graph graph (numNodes,numEdges, graph_file);
		graph.readGraph();
		vector<vector<vector<int>>> samples = sample(graph, 3);
		vector<vector<int>> cC = connectedComponents(&samples);
    }

void testConnectedComponnets2(){
		string graph_file = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file_certain.inf";
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
	string graph_file = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file.inf";
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
	string graph_file = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file_certain.inf";
	//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file_certain.inf";
	int numEdges = 11;
	int numNodes = 11;
	Graph graph (numNodes,numEdges, graph_file);
	graph.readGraph();
	vector<vector<vector<int>>> samples = sample(graph, 2);
	set<int> subgraph = {0,1};

	double reliability = subgraphReliability(samples, &subgraph);
	assert(reliability == 1);
}

//TODO test subgraph DFS
void testSubgraphDFS(){
	string graph_file = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file_certain.inf";
	//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file_certain.inf";
	int numEdges = 11;
	int numNodes = 11;
	Graph graph (numNodes,numEdges, graph_file);
	graph.readGraph();
	vector<vector<vector<int>>> samples = sample(graph, 1);
	
}

void testConnectedComponentsSubgraph(){
    // vector<vector<int>> components = connectedComponentsSubgraph(&graphSamples, m);
}


void testPeeling(){
	//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file2.inf";
	string graph_file = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file_certain.inf";
	set<set<int>> res = runPeeling(graph_file, 11, 11, 1, 0.5, 0);
	set<set<int>> expected = {{0, 1, 4, 3, 2, 5, 6}, {7,8}, {9,10}};
	assert(res == expected);
}

void testFastPeeling(){
	//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file2.inf";
	string graph_file = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file_certain.inf";
	set<set<int>> resPeeling = runPeeling(graph_file, 11, 11, 1, 0.5, 0);
	set<set<int>>  resFastPeeling = runFastPeeling(graph_file, 11, 11, 1, 0.5);
	set<set<int>> expected = {{0, 1, 4, 3, 2, 5, 6}, {7,8}, {9,10}};
	assert(resPeeling == expected);
	assert(resPeeling == resFastPeeling);
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
	//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file2.inf";
	string graph_file = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file3.inf";
	set<set<int>> res = runPeeling(graph_file, 7, 10, 100, 0.5, 0);
	set<set<int>> expected = {{0, 2, 3, 5}, {4, 6}};
	assert(res == expected);
}

void testFastPeelingNonDeterministic(){
	//string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file2.inf";
	string graph_file = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file3.inf";
	set<set<int>> res = runPeeling(graph_file, 7, 10, 100, 0.5, 0);
	set<set<int>> resFastPeeling = runFastPeeling(graph_file, 7, 10, 1000, 0.95);
	set<set<int>> expected = {{0, 2, 3, 5}, {4, 6}};
	assert(res == expected);
	assert(res == resFastPeeling);
	std::cout << "finished\n";

}

void testPeelingFacebook(){
	// string path = ".\\GraphsGeneration\\processed_graphs\\facebook_698.edges";
	string path = abspath + "\\BSc_project\\GraphsGeneration\\processed_graphs\\facebook_698.edges";
	set<set<int>> res = runPeeling(path,199, 270, 100, 0.95, 0);
	set<set<int>>  resFastPeeling = runFastPeeling(path, 199, 270, 100, 0.99);
	assert(res == resFastPeeling);
}

void testIterApriori(){
	vector<vector<int>> components = {{1,2}, {1,2,3},{1,4}};
	IterApriori iAp = IterApriori(&components, 3);
	for(int i = 0; i<4; i++){
		if(iAp.hasNext()){
			vector<int> frequentSet = iAp.getNextFrequentItemset();
			for (auto i : frequentSet){
				cout << i << ",";
			}
			cout << "\n";
		}
	}
	iAp.setMinSupport(0.1);
	for(int i = 0; i<10; i++){
		if(iAp.hasNext()){
			vector<int> frequentSet = iAp.getNextFrequentItemset();
			for (auto i : frequentSet){
				cout << i << ",";
			}
			cout << "\n";
		}
	}
}
void testIterApriori2(){
	vector<vector<int>> components = {{1,2}, {1,2,3},{1,4}};
	IterApriori iAp = IterApriori(&components, 3);
	for(int i = 0; i<4; i++){
		if(iAp.hasNext()){
			vector<int> frequentSet = iAp.getNextFrequentItemset();
			for (auto i : frequentSet){
				cout << i << ",";
			}
			cout << "\n";
		}
	}
	iAp.setMinSupport(0.1);
	for(int i = 0; i<4; i++){
		if(iAp.hasNext()){
			vector<int> frequentSet = iAp.getNextFrequentItemset();
			for (auto i : frequentSet){
				cout << i << ",";
			}
			cout << "\n";
		}
	}
	iAp.setMinSupport(0.5);
	if(iAp.hasNext()){
			vector<int> frequentSet = iAp.getNextFrequentItemset();
			for (auto i : frequentSet){
				cout << i << ",";
			}
			cout << "\n";
	}
}
void testTopKgraph3(){
	clock_t start;
	start = clock();
	// string path = ".\\GraphsGeneration\\processed_graphs\\facebook_698.edges";
	string path = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file4.inf";
	vector<Candidate> res = runTopKPeeling(path,7, 10, 1000, 1);
	vector<int> mostLikely = {2, 3, 5};
	double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "Time in topKFacebook: " << duration << "\n";
	assert(res[0].nodes == mostLikely);
	// set<set<int>> resFastPeeling = runFastPeeling(path, 199, 270, 100, 0.99);
	// assert(res == resFastPeeling);
	// {0, 2, 3, 5}, {4, 6}
}

void testTopKPeelingFacebook(){
	clock_t start;
	start = clock();
	// string path = ".\\GraphsGeneration\\processed_graphs\\facebook_698.edges";
	string path = abspath + "\\BSc_project\\GraphsGeneration\\processed_graphs\\facebook_698.edges";
	// string path = abspath + "/BSc_project/GraphsGeneration/processed_graphs/facebook_698.edges";
	vector<Candidate> res = runTopKPeeling(path,199, 270, 100, 1);
	vector<int> mostLikely = {75,103,48};
	double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "Time in topKFacebook: " << duration << "\n";
	cout << "FINAL TOP MOST RELIABLE PATTERN \n";
	printMFCS(res);
	// assert(res[0].nodes == mostLikely);
	// set<set<int>> resFastPeeling = runFastPeeling(path, 199, 270, 100, 0.99);
	// assert(res == resFastPeeling);
}

void testNaiveTopKPeelingGraph3() {
	string path = abspath + "\\BSc_project\\ReliablePaperImpl\\graph_file4.inf";
	set<NodesAndReliability> res = runNaiveTopKPeeling(path,7, 10, 1000, 2, 0.95, 0.02);
	set<int> mostLikely = {2, 3, 5};
	// assert(*res.begin() == mostLikely);
	// set<set<int>> resFastPeeling = runFastPeeling(path, 199, 270, 100, 0.99);
	// assert(res == resFastPeeling);
	// {0, 2, 3, 5}, {4, 6}
}

void testNaiveTopKPeelingFaceBook() {
	clock_t start;
	start = clock();
	string path = abspath + "\\BSc_project\\GraphsGeneration\\processed_graphs\\facebook_698.edges";
	set<NodesAndReliability> res = runNaiveTopKPeeling(path,199, 270, 100, 1, 0.97, 0.1);
	double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "Time in NaiveTopKPeelingFaceBook: " << duration << "\n";
	cout << "FINAL TOP MOST RELIABLE PATTERNS \n";
	printMFCSWhichAreNodesAndReliability(res);
}

void testNaiveTopKPeelingSynthetic() {
	clock_t start;
	start = clock();
	string path = abspath + "\\BSc_project\\GraphsGeneration\\processed_graphs\\syn_graph_seed0_n39_m80.edges";
	set<NodesAndReliability> res = runNaiveTopKPeeling(path,40, 80, 100, 2, 0.97, 0.1);
	double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "Time in NaiveTopKSynthetic: " << duration << "\n";
	cout << "FINAL TOP MOST RELIABLE PATTERNs \n";
	printMFCSWhichAreNodesAndReliability(res);
}

void testTopKPeelingSynthetic() {
	clock_t start;
	start = clock();
	string path = abspath + "\\BSc_project\\GraphsGeneration\\processed_graphs\\syn_graph_seed0_n39_m80.edges";
	vector<Candidate> res = runTopKPeeling(path,40, 80, 100, 2);
	double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "Time in TopKSynthetic: " << duration << "\n";
	cout << "FINAL TOP MOST RELIABLE PATTERNs \n";
	printMFCS(res);
}

// void testTopKReliableFacebook(){
	// string path = abspath + "\\BSc_project\\ReliablePaperImpl\\GraphsGeneration\\processed_graphs\\facebook_698.edges";
// 	vector<vector<int>> res = runTopKPeeling(path,199, 270, 100);
// 	// set<set<int>> resFastPeeling = runFastPeeling(path, 199, 270, 100, 0.95);
// 	// assert(res == resFastPeeling);
// }

void testAll(){
	// testPruneMatrix();
    // testPrune();
	// testApriori();
	// testConnectedComponnets();
	// testConnectedComponnets2();
	// testReliability();
	// testSampler();
	// testPeeling();
	// testPeelingNonDeterministic();
	// testSetSorting();
	// testFastPeeling();	
	// testFastPeelingNonDeterministic();
	// testPeelingFacebook();
	// testTopKReliableFacebook();
	// testIterApriori2();
	// testTopKgraph3();
	testTopKPeelingFacebook();
	// testNaiveTopKPeelingGraph3();
	// testNaiveTopKPeelingFaceBook();
	// testNaiveTopKPeelingSynthetic();
	// testTopKPeelingSynthetic();
}

