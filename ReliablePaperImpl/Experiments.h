#include "hoved.h"
#include "utility.h"
#include "apriori.h"
#include "peeling.h"
#include "fastPeeling.h"
#include "TopKPeeling.h"
#include "IterApriori.h"
#include "naiveTopKPeeling.h"
using namespace std;

struct ValueTime {
    double long time;
    double long value;
    ValueTime(double long _time, double long _value){
        time = _time;
        value = _value;
    }
};

ValueTime runExperiment(string path, int numSamples, int k){
    string line;
    string firstLine;
    string numNodes, numEdges;
    string problem;
    string value;
    ifstream graphFile (path);
    assert(graphFile.is_open());
    graphFile >> numNodes >> numEdges >> problem >> value;
    cout << value;
    Graph graph = Graph(stoi(numNodes), stoi(numEdges), "");
    graph.readGraphfile(&graphFile);
    clock_t start;
    vector<Candidate> result = runTopKPeelingWithoutSampling(graph, numSamples, k);
    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    return ValueTime(duration, stod(value));

}

void runNumNodesExperiment(){
    string FolderPath = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt\\BSc_project\\GraphsGeneration\\processed_graphs\\num_nodes_edgedegree2";
    for (int i = 6; i<7; i++){
        string filePath = FolderPath + "\\"+ to_string(i)+".txt";
        ValueTime valTime = runExperiment(filePath, 100, 1);
        cout << "finished value: "<< valTime.value << " in time: " << valTime.time <<"\n";
    }
}