#ifndef TOPKPSINGLESTEP_H
#define TOPKPSINGLESTEP_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "IterApriori.h"
#include "TopKPeeling.h"

vector<Candidate> topKSingleStep(vector<vector<vector<int>>> * graphSamples, vector<AggConComps> *components, int numSamples, int k, long double eps){
    vector<Candidate> tMFCS;
    long double theta = getThresholdOfRandomDFS(graphSamples, components, k);
    cout << "initial theta: " << theta;
    IterApriori iApriori = IterApriori(components, numSamples);
    iApriori.setMinSupport(max(theta - 0.01, long double (0.0)));
    int counter = 0;
    while (tMFCS.size() < k){
        vector<int> candidate = iApriori.getNextFrequentItemset();
        counter ++;
        if(candidate.size()>2){
            set<int> candidateSet = convertVectorToSet(candidate);
            double long reliability = subgraphReliability(*graphSamples, candidateSet, theta);
            if(reliability >= theta){
                tMFCS.push_back(Candidate(candidate, reliability));
                cout << reliability << "\n";
            }
        }
    }
    theta = tMFCS[getMinSupportIndex(tMFCS)].support;
    
    while(iApriori.hasNext()){
        counter++;
        Candidate canCandidate = getNextCandidateAndCheckReliability(iApriori, graphSamples, theta);
        if(canCandidate.nodes.size()==0){
            return tMFCS;
        } 
        if (canCandidate.support > theta){
            Candidate replacedElem = replaceLowestReliabilityMFCS(&tMFCS, canCandidate);
            int minIndex = getMinSupportIndex(tMFCS);
            long double minSupport = tMFCS[minIndex].support;
            theta = minSupport;
            cout <<"Theta: " << theta<<"\n"; 
            iApriori.setMinSupport(theta);            
        }
    }
    return tMFCS;
}

vector<Candidate> runTopKSingleStepWithoutSampling(vector<vector<vector<int>>>& samples, int numSamples, int k, long double eps){
    vector<vector<int>> components = connectedComponents(&samples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components, 2);
    vector<AggConComps> aggregatedComponents = aggregateConnectedComponentsSet(filteredComponents);
    return topKSingleStep(&samples, &aggregatedComponents, numSamples, k, eps);
}

vector<Candidate> runTopKSingleStep(string fileName, int k, long double eps, long double delta){
    Graph graph(fileName);
    int numSamples = calculateRequiredSamplesNewMethod(eps, delta, graph.numNodes, k);
    vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
    vector<Candidate> step1 = runTopKSingleStepWithoutSampling(graphSamples, numSamples, k, eps);
    return step1;
}
#endif





