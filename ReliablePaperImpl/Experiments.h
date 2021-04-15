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

ValueTime runExperiment(string path){
    string line;
    string firstLine;
    ifstream graphFile (path);
    if(graphFile.is_open()){
        getline(graphFile, firstLine);
        cout << firstLine;
        while( getline(graphFile, line)){
            cout << line <<"\n";
        }
    }
    return ValueTime(0, 0);
}

void runNumNodesExperiment(){
    string FolderPath = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt\\BSc_project\\GraphsGeneration\\processed_graphs\\edge_degree";
    for (int i = 0; i<1; i++){
        string filePath = FolderPath + "\\"+ to_string(i)+".txt";
        ValueTime time = runExperiment(filePath);
    }
}