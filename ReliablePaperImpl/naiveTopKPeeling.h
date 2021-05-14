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

vector<NodesAndReliability> naiveTopKPeeling(vector<vector<vector<int>>>& graphSamples, vector<vector<int>>& filteredComponents, int numSamples, int k, Graph& uncertain){
    long double thetaLow = 0;
    long double thetaHigh = 1;
    vector<NodesAndReliability> tempRes = {};
    while(tempRes.size() < k){
        long double thetaMid = (thetaHigh + thetaLow)/2;
        cout << "one iteration using threshold: " << thetaMid << "\n";
        set<set<int>> maximalFI = getMFI(filteredComponents, thetaMid, numSamples);
        vector<NodesAndReliability> MFCS = fastPeeling(graphSamples, maximalFI, thetaMid, numSamples);
        vector<vector<int>> MFCSNodes = extractNodes(MFCS);
        if(MFCSNodes.size() > 0){
            tempRes = runNonMaximal(graphSamples, uncertain, MFCSNodes, thetaMid);
            tempRes.insert(tempRes.end(), MFCS.begin(), MFCS.end());
        } 
        thetaHigh = thetaMid;
    }
    vector<NodesAndReliability> res = extractMax(tempRes, k);
    return res;
}

vector<NodesAndReliability> runNaiveTopKPeeling(string fileName, int numSamples, int k){
    Graph graph (fileName);
    vector<vector<vector<int>>> graphSamples = sample(graph, numSamples);
    return runNaiveTopKPeelingWithoutSampling(graphSamples, numSamples, k, graph);
}

vector<NodesAndReliability> runNaiveTopKPeelingWithoutSampling(vector<vector<vector<int>>>& samples, int numSamples, int k, Graph graph){
    vector<vector<int>> components = connectedComponents(&samples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components,2);
    return naiveTopKPeeling(samples, filteredComponents, numSamples, k, graph);
}

#endif