#ifndef TOPKPEELING_H
#define TOPKPEELING_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "IterApriori.h"

int getMinSupportIndex(vector<Candidate>& tMFCS){
    long double minIndex;
    long double minSupport=1.1;
    for (int i = 0; i<tMFCS.size(); i++){
        if(tMFCS[i].support < minSupport){
            minSupport = tMFCS[i].support;
            minIndex = i;
        }
    }
    return minIndex;
}


void replaceLowestReliabilityMFCS(vector<Candidate>* tMFCS, Candidate& replacee){
    int minIndex = getMinSupportIndex(*tMFCS);
    if(replacee.support > (*tMFCS)[minIndex].support){
        (*tMFCS)[minIndex] = replacee;
    }
}

Candidate getNextCandidateAndCheckReliability(IterApriori & iApriori,vector<vector<vector<int>>> * graphSamples, long double threshold){
    vector<int> candidate = iApriori.getNextFrequentItemset();
    set<int> candidateSet = convertVectorToSet(candidate);
    double long reliability = subgraphReliability(*graphSamples, &candidateSet, threshold);
    return Candidate(candidate, reliability);
}

long double getThresholdOfRandomDFS(vector<vector<vector<int>>> * graphSamples, vector<vector<int>> * components, int k){
    long double lowestBest = 1;
    for(int i = 0; i < k ; i++){
        set<int> firstComponent = convertVectorToSet((*components)[i]);
        long double res = subgraphReliability(*graphSamples, &firstComponent, 0);
        if(res < lowestBest){
            lowestBest = res;
        }
    }
    return lowestBest;
}

vector<Candidate> topKPeeling(vector<vector<vector<int>>> * graphSamples, vector<vector<int>> * components, int numSamples, int k){
    vector<Candidate> tMFCS;
    long double theta = 0;
    // long double theta = getThresholdOfRandomDFS(graphSamples, components, k)-0.00001;
    IterApriori iApriori = IterApriori(components, numSamples);
    iApriori.setMinSupport(theta);
    // cout << "adding initial candidates\n";
    int counter = 0;
    while (tMFCS.size() < k){
        vector<int> candidate = iApriori.getNextFrequentItemset();
        counter ++;
        if(counter % 4000 == 0){
            // cout << " 4000 iterations\n";
        }
        if(candidate.size()>2){
            set<int> candidateSet = convertVectorToSet(candidate);
            double long reliability = subgraphReliability(*graphSamples, &candidateSet, theta);
            if(reliability >= theta){
                tMFCS.push_back(Candidate(candidate, reliability));
            }
            // cout << "adding candidate\n";
        }
    }
    theta = tMFCS[getMinSupportIndex(tMFCS)].support;
    // cout << "done adding initial candidates\n";

    while(iApriori.hasNext()){
        counter++;
        if(counter % 40000 == 0){
        //    cout << " 40000 iterations in iApriori.hasNext() \n";
        }
        Candidate canCandidate = getNextCandidateAndCheckReliability(iApriori, graphSamples, theta);
        if(canCandidate.nodes.size()==0){
            return tMFCS;
        } 
        if (canCandidate.support > theta){
            replaceLowestReliabilityMFCS(&tMFCS, canCandidate);
            int minIndex = getMinSupportIndex(tMFCS);
            long double minSupport = tMFCS[minIndex].support;
            theta = minSupport;
            iApriori.setMinSupport(minSupport);
            cout << "Set new threshold: " << theta<< "\n";
        }
    }
    return tMFCS;
}

vector<Candidate> runTopKPeelingWithoutSampling(Graph graph, int numSamples, int k){
    vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
    vector<vector<int>> components = connectedComponents(&graphSamples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components, 2);

    return topKPeeling(&graphSamples, &components, numSamples, k);
}

vector<Candidate> runTopKPeeling(string fileName, int numNodes, int numEdges, int numSamples, int k){
    Graph graph(numNodes, numEdges, fileName);
    graph.readGraph();
    return runTopKPeelingWithoutSampling(graph, numSamples, k);
}
#endif





