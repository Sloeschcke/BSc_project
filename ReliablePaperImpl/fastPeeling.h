
#ifndef FASTPEELING_H
#define FASTPEELING_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "apriori.h"
#include "peeling.h"

using namespace std;

struct customCompareLength final{
    bool operator () (const set<int>& left, const set<int>& right) const
    {
        int nLeft = left.size();
        int nRight = right.size();
        return nLeft >= nRight;
    }
};

bool isContainedInPrevious(vector<int> component, set<set<int>> previous){
    //TODO, make components list of sets.
    set<int> componentSet(component.begin(), component.end());
    int count = previous.count(componentSet);
    return (count != 0);
}

vector<vector<int>> transReduce(vector<vector<vector<int>>> * graphSamples, set<int> m, set<set<int>> P, set<set<int>> MFCS){
    vector<vector<int>> components = connectedComponentsSubgraph(graphSamples, m);
    set<set<int>> cup = P;
    cup.insert(MFCS.begin(), MFCS.end());

    vector<vector<int>> result;
    for (auto component : components){
        if (!isContainedInPrevious(component, cup)){
            result.push_back(component);
        };
    };
    return result;
}

set<set<int>> fastPeeling(vector<vector<vector<int>>> graphSamples, set<set<int>> mfls, double threshold, set<set<int>> MFCS1, int numSamples){
    set<set<int>, customCompareLength> L;
	copy(mfls.begin(), mfls.end(), inserter(L, L.begin()));
    set<set<int>> newLayer = {};
    set<set<int>> MFCS = {};
    while(L.size() != 0){
        set<set<int>> P = {};
        for (auto m : L){
            if(subgraphReliability(graphSamples, m) >= threshold){ //check if m is a frequent cohesive set
                //no m' in MFCS where m is subset of m'
                if(shouldAddToMFCS(MFCS, m)){
                    MFCS.insert(m);
                    MFCS = prune(MFCS);
                }
            } else {
                vector<vector<int>> components = transReduce(&graphSamples, m, P, MFCS);
                set<set<int>> maximalFI = getMFI(components, threshold, numSamples);
                newLayer.insert(maximalFI.begin(), maximalFI.end());
                newLayer = prune(newLayer);
            }
            P.insert(m);
        }
        copy(MFCS.begin(), MFCS.end(), inserter(newLayer, newLayer.begin()));
        newLayer = prune(newLayer);
        for (auto m : MFCS){
            newLayer.erase(m);
        }
        //TODO make newlayer of customCompareLength
        L = {};
        copy(newLayer.begin(), newLayer.end(), inserter(L, L.begin()));
        newLayer = {};
    }
    return MFCS;
}



set<set<int>> runFastPeeling(string fileName, int numNodes, int numEdges, int numSamples, long double threshold){
    Graph graph (numNodes, numEdges, fileName);
	graph.readGraph();
    vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
    vector<vector<int>> components = connectedComponents(&graphSamples);
    
    set<set<int>> maximalFI = getMFI(components, threshold, numSamples);
    set<set<int>> res = fastPeeling(graphSamples, maximalFI, threshold, {}, numSamples);
    return res;
}

#endif
