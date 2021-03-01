
#ifndef FASTPEELING_H
#define FASTPEELING_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "apriori.h"
#include "peeling.h"

using namespace std;

bool shouldAddToMFCS(set<set<int>> MFCS, set<int> m){
    for (auto elem : MFCS){
        if(includes(elem.begin(), elem.end(), m.begin(), m.end())){
            return false;
        }
    }
    return true;
}

set<set<int>> fastPeeling(vector<vector<vector<int>>> graphSamples, set<set<int>> mfls, double threshold, set<set<int>> MFCS, int numSamples){
    set<int> newLayer = {};
    for (auto m : mfls){
        if(subgraphReliability(graphSamples, m) >= threshold){ //check if m is a frequent cohesive set
            //no m' in MFCS where m is subset of m'
            if(shouldAddToMFCS(MFCS, m)){
                MFCS.insert(m);
                MFCS = prune(MFCS);
            }
        } else {
            vector<vector<int>> components = connectedComponentsSubgraph(&graphSamples, m);
            set<set<int>> maximalFI = getMFI(components, threshold, numSamples);
            fastPeeling(graphSamples, maximalFI, threshold, MFCS, numSamples);
        }
    }
    return MFCS;
}


set<set<int>> runPeeling(string fileName, int numNodes, int numEdges, int numSamples, long double threshold){
    Graph graph (numNodes, numEdges, fileName);
	graph.readGraph();
    vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
    vector<vector<int>> components = connectedComponents(&graphSamples);
    
    set<set<int>> maximalFI = getMFI(components, threshold, numSamples);
    set<set<int>> res = fastPeeling(graphSamples, maximalFI, threshold, {}, numSamples);
    return res;
}

#endif
