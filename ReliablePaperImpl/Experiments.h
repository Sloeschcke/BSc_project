#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include "hoved.h"
#include "utility.h"
#include "apriori.h"
#include "peeling.h"
#include "fastPeeling.h"
#include "TopKPeeling.h"
#include "IterApriori.h"
#include "naiveTopKPeeling.h"
using namespace std;

// string abspath = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt\\BSc_project\\GraphsGeneration\\processed_graphs\\";
// string abspath = "/Users/sebastianloeschcke/Desktop/6.semester/BSc";
string abs_path = "C:\\Users\\chris\\Documents\\6. Semester\\Bachelor Project\\BSc_project\\GraphsGeneration\\processed_graphs\\";

struct ValueTime {
    double long time;
    double long value;
    ValueTime(double long _time, double long _value){
        time = _time;
        value = _value;
    }
};

ValueTime runExperiment(string path, int numSamples, int k){
    string numNodes, numEdges;
    string problem;
    string value;
    Graph graph = Graph(path);
    cout << graph.getValue() << "\n";
    vector<vector<vector<int>>> samples = sample(graph, numSamples);
    clock_t start;
    start = clock();
    vector<Candidate> result = runTopKPeelingWithoutSampling(samples, numSamples, k);
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

void runExperiment(bool valency, string category){
    string path = abs_path + category;
    vector<ValueTime> results;
    int numExperiments = 10;
    int numRepetitions = 5;
    for (int j = 0; j<numExperiments; j++){
        string folderPath = path +"\\" + to_string(j);
        if(valency){
            string folderPath = folderPath + "_valency";
        }
        for (int i = 0; i<numRepetitions; i++){
            string filePath = folderPath + "\\"+ to_string(i)+".txt";
            ValueTime valTime = runExperiment(filePath, 100, 2);
            cout << "finished value: "<< valTime.value << " in time: " << valTime.time <<"\n";
            results.push_back(valTime);
            cout << "============================\n";
            }
    }
    string resPath;
    if(valency){
        resPath =  path + "\\output\\results_valency.txt";
    } else {
        resPath = path + "\\output\\results.txt";
    }
    writeListOfResultsToFile(path+"/output/results.txt", results, "Number of nodes", "Nodes");
}
#endif

