
#ifndef PEELING_H
#define PEELING_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "Apriori.h"

using namespace std;

bool containsSupersetOfElem(set<set<int>> MFCS, set<int> m){
    for (auto & elem : MFCS){
        if(includes(elem.begin(), elem.end(), m.begin(), m.end())){
            return false;
        }
    }
    return true;
}

void checkReliabilityInGraphSamples2(set<set<int>>& MFCS, set<set<int>>& MFCS2, vector<vector<vector<int>>>& graphSamples2, long double threshold){
    for(auto & m : MFCS){
        double long reliability = subgraphReliability(graphSamples2, m, threshold);
        if(reliability >= threshold){
            MFCS2.insert(m);
        }
    }
}

void peeling(vector<vector<vector<int>>> graphSamples, set<set<int>> mfls, double threshold, set<set<int>> *MFCS, int numSamples){
    for (auto & m : mfls){
        if(subgraphReliability(graphSamples, m, threshold) >= threshold){ //check if m is a frequent cohesive set
            //no m' in MFCS where m is subset of m'
            if(containsSupersetOfElem(*MFCS, m)){ 
                (*MFCS).insert(m);
                vector<vector<int>> tempMFCS = setSetToVectorVector(*MFCS);
                tempMFCS = pruneVector(tempMFCS);
                *MFCS = vectorVectorToSetSet(tempMFCS);
            }
        } else {
            vector<vector<int>> components = connectedComponentsSubgraph(graphSamples, m);
            set<set<int>> maximalFI = getMFI(components, threshold, numSamples);
            peeling(graphSamples, maximalFI, threshold, MFCS, numSamples);
        }
    }
}



set<set<int>> runPeeling(string fileName, int numSamples, long double threshold, long double eps){
    Graph graph (fileName);
    vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
    vector<vector<vector<int>>> graphSamples2 = sample(graph, numSamples);
    vector<vector<int>> components = connectedComponents(&graphSamples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components, 1);

    
    set<set<int>> maximalFI = getMFI(filteredComponents, threshold-eps, numSamples);
    set<set<int>> MFCS;
    peeling(graphSamples, maximalFI, threshold, &MFCS, numSamples);

    set<set<int>> MFCS2;
    checkReliabilityInGraphSamples2(MFCS, MFCS2, graphSamples2, threshold+eps);
    return MFCS2;
}

#endif
