
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

struct NodesAndReliability final{
    vector<int> nodes;
    long double reliability;
    NodesAndReliability(vector<int> _nodes, long double _reliability){
        nodes = _nodes;
        reliability = _reliability;
    }

    const bool operator < (const NodesAndReliability &r) const{
        return (reliability < r.reliability);
    }
};

vector<vector<int>> extractNodes(vector<NodesAndReliability>& nodesAndReliability){
    vector<vector<int>> res = {};
    for(auto ele : nodesAndReliability){
        res.push_back(ele.nodes);
    }
    return res;
}

bool isContainedInPrevious(vector<int> component, set<set<int>> previous){
    set<int> componentSet(component.begin(), component.end());
    int count = previous.count(componentSet);
    return (count != 0);
}

vector<vector<int>> transReduce(vector<vector<vector<int>>>& graphSamples, set<int> m, set<set<int>> P, set<set<int>> MFCS){
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
vector<NodesAndReliability> fastPeeling(vector<vector<vector<int>>>& graphSamples, set<set<int>>& mfls, double threshold, int numSamples){
    int counter = 0;
    set<set<int>, customCompareLength> L;
	copy(mfls.begin(), mfls.end(), inserter(L, L.begin()));
    set<set<int>> newLayer = {};
    set<set<int>> MFCS = {};
    vector<NodesAndReliability> nodesAndReliability = {};
    while(L.size() != 0){
        counter ++;
        if(counter % 1000 == 0){
            cout << "1000 iterations!\n";
        }
        set<set<int>> P = {};
        for (auto m : L){
            long double reliability = subgraphReliability(graphSamples, &m, threshold);
            if(reliability >= threshold){ //check if m is a frequent cohesive set
                //no m' in MFCS where m is subset of m'
                if(containsSupersetOfElem(MFCS, m)){
                    vector<int> vector_m = convertSetToVector(m);
                    nodesAndReliability.push_back(NodesAndReliability(vector_m, reliability));
                    MFCS.insert(m);
                    vector<vector<int>> tempMFCS = setSetToVectorVector(MFCS);
                    tempMFCS = pruneVector(tempMFCS);
                    MFCS = vectorVectorToSetSet(tempMFCS);
                }
            } else {
                vector<vector<int>> components = transReduce(graphSamples, m, P, MFCS);
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
        L = {};
        copy(newLayer.begin(), newLayer.end(), inserter(L, L.begin()));
        newLayer = {};
    }
    return nodesAndReliability;
}



vector<NodesAndReliability> runFastPeeling(string fileName, int numSamples, long double threshold){
    Graph graph (fileName);
    vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
    vector<vector<int>> components = connectedComponents(&graphSamples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components,2);

    set<set<int>> maximalFI = getMFI(filteredComponents, threshold, numSamples);
    vector<NodesAndReliability> res = fastPeeling(graphSamples, maximalFI, threshold, numSamples);
    return res;
}

#endif
