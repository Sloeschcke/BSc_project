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

        if(tempRes.size() > 0){
            int innerCounter = 0;
            while(tempRes.size() < k){
                long double tmpThreshold = (1-currentThreshold)/2+currentThreshold; //binary increase of threshold
                tempRes = fastPeeling(graphSamples, maximalFI, currentThreshold, numSamples);
                bool hasSubsetSizeAbove3;
                for (auto &subset : tempRes){
                    if(subset.nodes.size() >3){
                        tmpThreshold = (1-tmpThreshold)/2+tmpThreshold;//increase thresshold
                        continue;
                    }
                }
                if (!hasSubsetSizeAbove3){
                    tmpThreshold = (tmpThreshold - currentThreshold)/2 + currentThreshold; //binary dreacrease of threshold
                }
                //check subsets of mfcs

            }
        }
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

set<NodesAndReliability> extractMax(set<NodesAndReliability> tempRes, int k){
    set<NodesAndReliability> res = {};
    auto it = tempRes.end();
    while (res.size() < k){
        it--;
        res.insert(*it);
    }
    return res;
}

set<NodesAndReliability> naiveTopKPeeling(vector<vector<vector<int>>>& graphSamples, vector<vector<int>>& filteredComponents, int numSamples, int k){
    long double infSmaller = 1/(graphSamples.size()*10);
    set<NodesAndReliability> tempRes = {};
    long double currentThreshold = 0.5;
    long double oldThreshold = 1;
    while(tempRes.size() < k){
        set<set<int>> maximalFI = getMFI(filteredComponents, currentThreshold, numSamples);
        tempRes = fastPeeling(graphSamples, maximalFI, currentThreshold, numSamples);
        if (tempRes.size() > 0){
            bool allOfSizeThree = true;
            for(auto& subgraph: tempRes){
                if (subgraph.nodes.size() > 3){
                    allOfSizeThree = false;
                    break;
                }
            }
            if(allOfSizeThree){
                if(tempRes.size() >= k){
                    return extractMax(tempRes, k);
                }   
                //decrease threshold
                oldThreshold = currentThreshold;
                currentThreshold = (currentThreshold - oldThreshold)/2 + oldThreshold; 
                continue;
            }
            auto it = tempRes.end();
            it--;
            long double maxReliabilityInRes = (*it).reliability;
            oldThreshold = currentThreshold;
            currentThreshold = maxReliabilityInRes + infSmaller;
            tempRes = {}; // make empty to run while loop again
        }
    }
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