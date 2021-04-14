#ifndef NAIVETOPKPEELING_H
#define NAIVETOPKPEELING_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "apriori.h"
#include "fastPeeling.h"
#include "TopKPeeling.h"

set<NodesAndReliability> naiveTopKPeeling(vector<vector<vector<int>>>& graphSamples, vector<vector<int>>& filteredComponents, int numSamples, long double initialThreshold, long double stepSize, int k){
    int counter = 1;
    set<NodesAndReliability> tempRes = {};
    long double currentThreshold = initialThreshold;
    while(tempRes.size() < k && currentThreshold >= 0){
        cout << "running naiv top k with " << currentThreshold << "\n";
        set<set<int>> maximalFI = getMFI(filteredComponents, currentThreshold, numSamples);
        tempRes = fastPeeling(graphSamples, maximalFI, currentThreshold, numSamples);
        currentThreshold = initialThreshold - stepSize*counter;
        if(currentThreshold < 0){
            currentThreshold = getThresholdOfRandomDFS(&graphSamples, &filteredComponents, k);
        }
        counter++;
    }
    if(tempRes.size() <= k){
        return tempRes;
    }

    set<NodesAndReliability> res = {};
    auto it = tempRes.end();
    while (res.size() < k){
        it--;
        res.insert(*it);
    }
    return res;
}

set<NodesAndReliability> runNaiveTopKPeeling(string fileName, int numNodes, int numEdges, int numSamples, int k, long double initialThreshold, long double stepSize){
    Graph graph (numNodes, numEdges, fileName);
	graph.readGraph();
    vector<vector<vector<int>>> graphSamples = sample(graph, numSamples);
    vector<vector<int>> components = connectedComponents(&graphSamples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components,2);

    set<NodesAndReliability> res = naiveTopKPeeling(graphSamples, filteredComponents, numSamples, initialThreshold, stepSize, k);
    return res;
}

#endif