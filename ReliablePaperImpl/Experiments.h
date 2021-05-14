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

string abs_path = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt\\BSc_project\\GraphsGeneration\\processed_graphs\\";
// string abspath = "/Users/sebastianloeschcke/Desktop/6.semester/BSc";
// string abs_path = "C:\\Users\\chris\\Documents\\6. Semester\\Bachelor Project\\BSc_project\\GraphsGeneration\\processed_graphs\\";

struct ValueTime {
    double long time;
    double long value;
    ValueTime(double long _time, double long _value){
        time = _time;
        value = _value;
    }
};

ValueTime runExperiment(string path, int numSamples, int k){
    clock_t start;
    start = clock();
    Graph graph = Graph(path);
    string value = graph.getValue();
    cout << value << "\n";
    vector<vector<vector<int>>> samples = sample(graph, numSamples);
    resultMFCS result = runTopKPeelingWithoutSampling(samples, numSamples, k, 0.001);
    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    return ValueTime(duration, stod(value));
}

ValueTime runNaiveExperiment(string path, int numSamples, int k){
    clock_t start;
    start = clock();
    Graph graph = Graph(path);
    string value = graph.getValue();
    cout << value << "\n";
    vector<vector<vector<int>>> samples = sample(graph, numSamples);
    vector<NodesAndReliability> result = runNaiveTopKPeelingWithoutSampling(samples, numSamples, k, graph);
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

void runExperiments(bool valency, bool naive, string category){
    string path = abs_path + category;
    vector<ValueTime> results;
    int numExperiments = 10;
    int numRepetitions = 5;
    for (int j = 1; j<numExperiments; j++){
        string folderPath = path +"\\" + to_string(j);
        if(valency){
            folderPath = folderPath + "_valency";
        }
        for (int i = 0; i<numRepetitions; i++){
            string filePath = folderPath + "\\"+ to_string(i)+".txt";
            double long value;
            double long time;
            if(!naive){
                if(category == "Varying K"){
                    ValueTime valTime = runExperiment(filePath, 100, i + 1);
                    results.push_back(valTime);
                    value = valTime.value;
                    time = valTime.time;
                } else {
                    ValueTime valTime = runExperiment(filePath, 100, 2);
                    results.push_back(valTime);
                    value = valTime.value;
                    time = valTime.time;
                }
            } else {
                if(category == "Varying K"){
                    ValueTime valTime = runNaiveExperiment(filePath, 100, i + 1);
                    results.push_back(valTime);
                    value = valTime.value;
                    time = valTime.time;
                } else {
                    ValueTime valTime = runNaiveExperiment(filePath, 100, 2);
                    results.push_back(valTime);
                    value = valTime.value;
                    time = valTime.time;
                }
            }
            cout << "finished value: "<< value << " in time: " << time <<"\n";
            cout << "============================\n";
        }
    }

    string resPath;
    if(!naive){
        if(valency){
            resPath =  path + "\\output\\results_valency.txt";
        } else {
            resPath = path + "\\output\\results.txt";
        }
    } else {
        if(valency){
            resPath =  path + "\\output\\results_naive_valency.txt";
        } else {
            resPath = path + "\\output\\results_naive.txt";
        }
    }
    writeListOfResultsToFile(resPath, results, category, category);
}

void runSimpleExperiment(){
    vector<ValueTime> results;
    string path = abs_path + "\\num_nodes\\0\\0.txt";
    ValueTime valTime = runExperiment(path, 100, 2);
    results.push_back(valTime);
    string resPath = abs_path + "\\num_nodes\\output\\results.txt";
    writeListOfResultsToFile(resPath, results, "test", "test");
}

#endif

