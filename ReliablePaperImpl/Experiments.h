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
    cout << value << "\n";
    Graph graph = Graph(stoi(numNodes), stoi(numEdges), "");
    graph.readGraphfile(&graphFile);
    clock_t start;
    start = clock();
    vector<Candidate> result = runTopKPeelingWithoutSampling(graph, numSamples, k);
    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    return ValueTime(duration, stod(value));

}

void writeListOfResultsToFile(string outPath, vector<ValueTime> results, string graphTitle, string xLabel){
    ofstream outFile(outPath);
    outFile << graphTitle <<"\n";
    outFile << xLabel << "\n";
    for (auto & result : results){
        string formattedString = to_string(result.value) + " " + to_string(result.time) + "\n";
        outFile << formattedString;
    }
    outFile.close();
}

void runNumNodesExperiment(){
    string path = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt\\BSc_project\\GraphsGeneration\\processed_graphs\\num_nodes_edgedegree2";
    vector<ValueTime> results;
    int numExperiments = 10;
    int numRepetitions = 5;
    for (int j = 0; j<numExperiments; j++){
        string folderPath = path +"/" + to_string(j);
        for (int i = 0; i<numRepetitions; i++){
            string filePath = folderPath + "\\"+ to_string(i)+".txt";
            ValueTime valTime = runExperiment(filePath, 100, 2);
            cout << "finished value: "<< valTime.value << " in time: " << valTime.time <<"\n";
            results.push_back(valTime);
            cout << "============================\n";
            }
    }
    writeListOfResultsToFile(path+"/output/results.txt", results, "Number of nodes", "Nodes");
}

void runKExperiment(){
    string path = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt\\BSc_project\\GraphsGeneration\\processed_graphs\\Varying_K\\1";
    vector<ValueTime> results;
    cout << "1";
    int numRepetitions = 5;
    vector<int> kExperiments = {1,2,3,4,5,6,7,8,9,10};
    cout << "2";
    for (auto & k : kExperiments ){
        for (int i=0; i<numRepetitions; i++){
            cout << "3";
            string filePath = path + "\\" + to_string(i) + ".txt";
            ValueTime valTime = runExperiment(filePath, 1000,k);
            valTime.value = k;
            cout << "finished value: "<< valTime.value << " in time: " << valTime.time <<"\n";
            results.push_back(valTime);
            cout << "============================\n";
        }
    }
    writeListOfResultsToFile(path+"/output/results.txt", results, "Vary k", "k");

}
