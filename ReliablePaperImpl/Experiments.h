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
    vector<Candidate> result;
    ValueTime(double long _time, double long _value, vector<Candidate> _result){
        result = _result;
        time = _time;
        value = _value;
    }
};

ValueTime run2StepExperiment(string path, int k, long double eps, long double delta){
    Graph graph = Graph(path);
    string value = graph.getValue();
    cout << value << "\n";

    clock_t start;
    start = clock();
    resultMFCS result = runTopKPeeling(path,k, eps, delta);
    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    return ValueTime(duration, stod(value), result.MFCS);
}

ValueTime runSingleStepExperiment(string path, int k, long double eps, long double delta){
    Graph graph = Graph(path);
    string value = graph.getValue();
    cout << value << "\n";

    clock_t start;
    start = clock();
    vector<Candidate> result = runTopKSingleStep(path,k, eps, delta);
    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    return ValueTime(duration, stod(value), result);
}

ValueTime runNaiveExperiment(string path, int k, long double eps, long double delta){
    clock_t start;
    Graph graph = Graph(path);
    string value = graph.getValue();
    cout << value << "\n";
    
    start = clock();
    vector<Candidate> result = runNaiveTopKPeeling(path, k, eps, delta);
    double duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    return ValueTime(duration, stod(value), result);
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

string getResPath(bool valency, string algorithm, string path){
    string resPath;
    if(algorithm == "2Step"){
        if(valency){
            resPath =  path + "\\output\\results_2step_valency.txt";
        } else {
            resPath = path + "\\output\\results_2step.txt";
        }
    } else if(algorithm == "Naive") {
        if(valency){
            resPath =  path + "\\output\\results_naive_valency.txt";
        } else {
            resPath = path + "\\output\\results_naive.txt";
        }
    } else if(algorithm == "1Step") {
        if(valency){
            resPath =  path + "\\output\\results_1step_valency.txt";
        } else {
            resPath = path + "\\output\\results_1step.txt";
        }
    }
    return resPath;
}

void runExperiments(bool valency, string algorithm, string category, long double eps, long double delta, int k){
    string path = abs_path + category;
    string resPath = getResPath(valency, algorithm, path);
    vector<ValueTime> results;
    int numExperiments = 10;
    int numRepetitions = 5;
    for (int j = 0; j < numExperiments; j++){
        string folderPath = path +"\\" + to_string(j);
        if(valency){
            folderPath = folderPath + "_valency";
        }
        double duration;
        for (int i = 0; i<numRepetitions; i++){
            clock_t start;
            start = clock();
            string filePath = folderPath + "\\"+ to_string(i)+".txt";
            double long value;
            double long time;
            if(algorithm == "2Step"){
                if(category == "Varying_K"){
                    ValueTime valTime = run2StepExperiment(filePath, i + 1, eps, delta);
                    results.push_back(valTime);
                    value = i+1;
                    time = valTime.time;
                } else {
                    ValueTime valTime = run2StepExperiment(filePath, k, eps, delta);
                    results.push_back(valTime);
                    value = valTime.value;
                    time = valTime.time;
                }
            } if(algorithm=="Naive") {
                if(category == "Varying_K"){
                    ValueTime valTime = runNaiveExperiment(filePath, i + 1, eps, delta);
                    results.push_back(valTime);
                    value = i+1;
                    time = valTime.time;
                } else {
                    ValueTime valTime = runNaiveExperiment(filePath, k, eps, delta);
                    results.push_back(valTime);
                    value = valTime.value;
                    time = valTime.time;
                }
            }
            if(algorithm == "1Step"){
                if(category == "Varying_K"){
                    ValueTime valTime = runSingleStepExperiment(filePath, i + 1, eps, delta);
                    results.push_back(valTime);
                    value = i+1;
                    time = valTime.time;
                } else {
                    ValueTime valTime = runSingleStepExperiment(filePath, k, eps, delta);
                    results.push_back(valTime);
                    value = valTime.value;
                    time = valTime.time;
                }
            }
            cout << "finished value: "<< value << " in time: " << time <<"\n";
            cout << "============================\n";

            duration = max(duration, ( clock() - start ) / (double) CLOCKS_PER_SEC);
        }
        if(duration > 300){
                writeListOfResultsToFile(resPath, results, category, category);
                return;
            }
    }
    writeListOfResultsToFile(resPath, results, category, category);
}

void allNumNodesRunExperiments(){
    long double eps = 0.05;
    long double delta = 0.01;
    int k = 3;
    runExperiments(true, "Naive", "num_nodes", eps, delta, k);
    runExperiments(true, "2Step", "num_nodes", eps, delta, k);
    runExperiments(false, "1Step", "num_nodes", eps, delta, k);
    runExperiments(false, "2Step", "num_nodes", eps, delta, k);
    runExperiments(false, "Naive", "num_nodes", eps, delta, k);
    runExperiments(true, "1Step", "num_nodes", eps, delta, k);
}

void allVaryingKRunExperiments(){
    long double eps = 0.05;
    long double delta = 0.01;
    int k = 3;
    // runExperiments(false, "2Step", "Varying_K", eps, delta, k);
    // runExperiments(false, "1Step", "Varying_K", eps, delta, k);
    // runExperiments(false, "Naive", "Varying_K", eps, delta, k);
    runExperiments(true, "2Step", "Varying_K", eps, delta, k);
    runExperiments(true, "1Step", "Varying_K", eps, delta, k);
    // runExperiments(true, "Naive", "Varying_K", eps, delta, k);
}

void allEdgeDegreeRunExperiments(){
    long double eps = 0.05;
    long double delta = 0.01;
    int k = 3;
    runExperiments(false, "2Step", "edge_degree", eps, delta, k);
    runExperiments(false, "1Step", "edge_degree", eps, delta, k);
    runExperiments(true, "2Step", "edge_degree", eps, delta, k);
    runExperiments(true, "1Step", "edge_degree", eps, delta, k);
    // runExperiments(false, "Naive", "edge_degree", eps, delta, k);
    // runExperiments(true, "Naive", "edge_degree", eps, delta, k);
}

// void runSimpleExperiment(){
//     vector<ValueTime> results;
//     string path = abs_path + "\\num_nodes\\0\\0.txt";
//     ValueTime valTime = runExperiment(path, 100, 2);
//     results.push_back(valTime);
//     string resPath = abs_path + "\\num_nodes\\output\\results.txt";
//     writeListOfResultsToFile(resPath, results, "test", "test");
// }

#endif

