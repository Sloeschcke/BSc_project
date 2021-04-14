
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

// struct NodesAndReliability final{
//     set<set<int>> setsOfNodes;
//     set<long double> reliabilities;
//         NodesAndReliability(set<set<int>> _nodes, set<double long> _reliability){
//             setsOfNodes = _nodes;
//             reliabilities = _reliability;
//     } 
// };

struct NodesAndReliability final{
    set<int> nodes;
    long double reliability;
    NodesAndReliability(set<int> _nodes, long double _reliability){
        nodes = _nodes;
        reliability = _reliability;
    }

    const bool operator < (const NodesAndReliability &r) const{
        return (reliability < r.reliability);
    }
};

set<set<int>> extractNodes(set<NodesAndReliability>& nodesAndReliability){
    set<set<int>> res = {};
    for(auto ele : nodesAndReliability){
        res.insert(ele.nodes);
    }
    return res;
}

bool isContainedInPrevious(vector<int> component, set<set<int>> previous){
    //TODO, make components list of sets.
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
set<NodesAndReliability> fastPeeling(vector<vector<vector<int>>>& graphSamples, set<set<int>> mfls, double threshold, int numSamples){
    int counter = 0;
    set<set<int>, customCompareLength> L;
	copy(mfls.begin(), mfls.end(), inserter(L, L.begin()));
    set<set<int>> newLayer = {};
    set<set<int>> MFCS = {};
    set<NodesAndReliability> nodesAndReliability = {};
    while(L.size() != 0){
        counter ++;
        if(counter % 100 == 0){
            cout << "100 iterations!";
        }
        set<set<int>> P = {};
        for (auto m : L){
            long double reliability = subgraphReliability(graphSamples, &m);
            if(reliability >= threshold){ //check if m is a frequent cohesive set
                //no m' in MFCS where m is subset of m'
                if(containsSupersetOfElem(MFCS, m)){
                    nodesAndReliability.insert(NodesAndReliability(m, reliability));
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
        //TODO make newlayer of customCompareLength
        L = {};
        copy(newLayer.begin(), newLayer.end(), inserter(L, L.begin()));
        newLayer = {};
    }
    return nodesAndReliability;
}



set<set<int>> runFastPeeling(string fileName, int numNodes, int numEdges, int numSamples, long double threshold){
    Graph graph (numNodes, numEdges, fileName);
	graph.readGraph();
    vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
    vector<vector<int>> components = connectedComponents(&graphSamples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components,2);

    set<set<int>> maximalFI = getMFI(filteredComponents, threshold, numSamples);
    set<NodesAndReliability> peelingRes = fastPeeling(graphSamples, maximalFI, threshold, numSamples);
    set<set<int>> res = extractNodes(peelingRes);
    return res;
}

#endif
