#ifndef NAIVETOPKPEELING_H
#define NAIVETOPKPEELING_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "apriori.h"
#include "fastPeeling.h"
#include "TopKPeeling.h"
#include "MiningNonMaximal.h"

vector<NodesAndReliability> extractMax(vector<NodesAndReliability> tempRes, int k){
    sort(tempRes.begin(), tempRes.end());
    vector<NodesAndReliability> res = {};
    auto it = tempRes.end();
    while (res.size() < k){
        it--;
        res.push_back(*it);
    }
    return res;
}

vector<NodesAndReliability> naiveTopKPeeling(vector<vector<vector<int>>>& graphSamples, vector<vector<int>>& filteredComponents, int numSamples, long double initialThreshold, long double stepSize, int k, Graph& uncertain){
    vector<NodesAndReliability> tempRes = {};
    long double currentThreshold = initialThreshold;
    while(tempRes.size() < k){
        set<set<int>> maximalFI = getMFI(filteredComponents, currentThreshold, numSamples);
        vector<NodesAndReliability> MFCS = fastPeeling(graphSamples, maximalFI, currentThreshold, numSamples);
        vector<vector<int>> MFCSNodes = extractNodes(MFCS);
        if(MFCSNodes.size() > 0){
            tempRes = runNonMaximal(graphSamples, uncertain, MFCSNodes, currentThreshold);
            tempRes.insert(tempRes.end(), MFCS.begin(), MFCS.end());
        }
        currentThreshold = currentThreshold - stepSize;
    }
    vector<NodesAndReliability> res = extractMax(tempRes, k);
    return res;
}

vector<NodesAndReliability> runNaiveTopKPeeling(string fileName, int numNodes, int numEdges, int numSamples, int k, long double initialThreshold, long double stepSize){
    Graph graph (numNodes, numEdges, fileName);
	graph.readGraph();
    vector<vector<vector<int>>> graphSamples = sample(graph, numSamples);
    vector<vector<int>> components = connectedComponents(&graphSamples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components,2);

    vector<NodesAndReliability> res = naiveTopKPeeling(graphSamples, filteredComponents, numSamples, initialThreshold, stepSize, k, graph);
    return res;
}

#endif