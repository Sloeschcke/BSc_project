#ifndef NAIVETOPKPEELING_H
#define NAIVETOPKPEELING_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "apriori.h"
#include "fastPeeling.h"

set<set<int>> runNaiveTopKPeeling(string fileName, int numNodes, int numEdges, int numSamples, int k, long double initialThreshold, long double stepSize){
    Graph graph (numNodes, numEdges, fileName);
	graph.readGraph();
    vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
    vector<vector<int>> components = connectedComponents(&graphSamples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components,2);

    set<set<int>> res = naiveTopKPeeling(graphSamples, filteredComponents, numSamples, initialThreshold, stepSize, k);
    return res;
}


set<set<int>> naiveTopKPeeling(vector<vector<vector<int>>>& graphSamples, vector<vector<int>>& filteredComponents, int numSamples, long double initialThreshold, long double stepSize, int k){
    int counter = 0;
    set<set<int>> res = {};
    while(res.size() < k){
        long double currentThreshold = initialThreshold - stepSize* pow(2,counter);
        set<set<int>> maximalFI = getMFI(filteredComponents, currentThreshold, numSamples);
        counter++;
    }
}
#endif