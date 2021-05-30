#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include "hoved.h"
#include "utility.h"
#include "Apriori.h"
#include "peeling.h"
#include "fastPeeling.h"
#include "TopKPeeling.h"
#include "IterApriori.h"
#include "naiveTopKPeeling.h"
#include "TopKSingleStep.h"

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

void printMFCS2(vector<Candidate> MFCS){
	for (auto elem: MFCS)
	{
		for(auto elem2: elem.nodes){
			cout << elem2 << ", " ;
		}
		cout << "with reliability: " << elem.support << "\n";
	}
}

ValueTime run2StepExperiment(string path, int k, long double eps, long double delta){
    Graph graph = Graph(path);
    string value = graph.getValue();
    cout << value << "\n";

    clock_t start;
    start = clock();
    resultMFCS result = runTopKPeeling(path,k, eps, delta, 0.001);
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
    printMFCS2(result);
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
struct compareCandidates{
    Candidate key;
    compareCandidates(Candidate &i): key(i){}

    bool operator()(Candidate &i){
        sort(key.nodes.begin(), key.nodes.end());
        sort(i.nodes.begin(), i.nodes.end());
        return key.nodes == i.nodes;
    }
};

int findWrongsBetweenMFCS(vector<Candidate> result2Step, vector<Candidate> result1Step){
    int counter = 0;
    for (auto & candidate : result1Step){
       if (find_if(result2Step.begin(), result2Step.end(), compareCandidates(candidate)) == result2Step.end()) {
            cout << "not found";
            counter++;
        } 
    }
    return counter;
}

void runExperiments(bool valency, string algorithm, string category, long double eps, long double delta, int k){
    string path = abs_path + category;
    string resPath = getResPath(valency, algorithm, path);
    vector<ValueTime> results;
    vector<int> k_values = {1,2,3,5,10,20,40,60,80,100};
    int numExperiments = 5;
    int numRepetitions = 2;
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
                    value = k_values[j];
                    ValueTime val = run2StepExperiment(filePath, k_values[j], eps, delta);
                    ValueTime valTime = ValueTime(val.time, value, val.result);
                    results.push_back(valTime);
                    time = valTime.time;
                } else {
                    ValueTime valTime = run2StepExperiment(filePath, k, eps, delta);
                    results.push_back(valTime);
                    value = valTime.value;
                    time = valTime.time;
                }
            } if(algorithm=="Naive") {
                if(category == "Varying_K"){
                    value = k_values[j];
                    ValueTime val = runNaiveExperiment(filePath, k_values[j], eps, delta);
                    ValueTime valTime = ValueTime(val.time, value, val.result);
                    results.push_back(valTime);
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
                    value = k_values[j];
                    ValueTime val = runSingleStepExperiment(filePath, k_values[j], eps, delta);
                    ValueTime valTime = ValueTime(val.time, value, val.result);
                    results.push_back(valTime);
                    time = valTime.time;
                } else {
                    ValueTime valTime = runSingleStepExperiment(filePath, k, eps, delta);
                    results.push_back(valTime);
                    value = valTime.value;
                    time = valTime.time;
                }
            }
            writeListOfResultsToFile(resPath, results, category, category);
            cout << "finished value: "<< value << " in time: " << time <<"\n";
            cout << "============================\n";

            duration = max(duration, ( clock() - start ) / (double) CLOCKS_PER_SEC);
        }
        // if(duration > 300){
        //         writeListOfResultsToFile(resPath, results, category, category);
        //         return;
        // }
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

void runNumNodesFDist(){
    long double eps = 0.05;
    long double delta = 0.01;
    int k = 1;
    runExperiments(false, "1Step", "num_nodes_f_dist", eps, delta, k);
}

void run2StepAll(){
    long double epsLimit = 0.0005;
    long double eps = 0.05;
    long double delta = 0.01;
    int k = 3;
    runExperiments(false, "2Step", "num_nodes", eps, delta, k);
    runExperiments(true, "2Step", "num_nodes", eps, delta, k);
    runExperiments(false, "2Step", "edge_degree", eps, delta, k);
    runExperiments(true, "2Step", "edge_degree", eps, delta, k);
    runExperiments(false, "2Step", "delta", eps, delta, k);
    runExperiments(true, "2Step", "delta", eps, delta, k);
    runExperiments(true, "2Step", "Varying_K", eps, delta, k);
    runExperiments(false, "2Step", "Varying_K", eps, delta, k);
}

void allVaryingKRunExperiments(){
    long double eps = 0.05;
    long double delta = 0.01;
    int k = 3;
    runExperiments(false, "2Step", "Varying_K", eps, delta, k);
    runExperiments(false, "1Step", "Varying_K", eps, delta, k);
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

void runAllNaiveExperiments(){
    long double eps = 0.05;
    long double delta = 0.01;
    int k = 3;
    runExperiments(false, "Naive", "edge_degree", eps, delta, k);
    runExperiments(true, "Naive", "edge_degree", eps, delta, k);
    runExperiments(false, "Naive", "Varying_K", eps, delta, k);
    runExperiments(true, "Naive", "Varying_K", eps, delta, k);
    runExperiments(true, "Naive", "num_nodes", eps, delta, k);
    runExperiments(false, "Naive", "num_nodes", eps, delta, k);
}



void Precision2Step(){
    int numExperiments = 10;
    int numIterations = 5;
    int k=10;
    double long eps = 0.05;
    double long delta = 0.01;
    double long epsLimit = 0.001;

    vector<ValueTime> results;
    string path;
    for (int i = 0; i<numExperiments; i++){
        for (int j = 0; j<numIterations; j++){
            path = abs_path +"num_nodes\\" + to_string(i);
            path = path + "\\"+ to_string(j)+".txt";
            cout << path;
            Graph graph = Graph(path);

            string value = graph.getValue();
            cout << value << "\n";

            vector<Candidate> result1Step = runTopKSingleStep(path, k, eps, delta);
            resultMFCS result2Step = runTopKPeeling(path, k, eps, delta, epsLimit);
            int wrongs = findWrongsBetweenMFCS(result2Step.MFCS, result1Step);
            results.push_back(ValueTime(wrongs, stod(value), result2Step.MFCS));
        }
    writeListOfResultsToFile(abs_path+"PrecisionNumNodes\\output\\result.txt",results, "number wrong with k = 10", "num nodes");
}
}



void ToyDataSetValidation(){
    int numExperiments = 5;
    int numIterations = 5;
    int k=5;
    double long eps = 0.05;
    double long delta = 0.01;
    double long epsLimit = 0.0001;

    vector<ValueTime> results;
    string path;
    path = abs_path +"ToyGraph\\graph_file5.inf";
    cout << path;
    Graph graph = Graph(path);

    string value = graph.getValue();
    cout << value << "\n";

    vector<Candidate> result1Step = runTopKSingleStep(path, k, eps, delta);
    resultMFCS result2Step = runTopKPeeling(path, k, eps, delta, epsLimit);
    cout << "1 step ========================\n";
    printMFCS2(result1Step);
    cout << "2 step ========================\n";
    printMFCS2(result2Step.MFCS);
    int wrongs = findWrongsBetweenMFCS(result2Step.MFCS, result1Step);
    cout << wrongs;
    results.push_back(ValueTime(wrongs, stod(value), result2Step.MFCS));
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

