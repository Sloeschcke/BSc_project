#ifndef TOPKPEELING_H
#define TOPKPEELING_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "IterApriori.h"

struct resultMFCS{
    vector<Candidate> MFCS;
    vector<Candidate> MFCSBuffer;
    long double theta;
    long double thetaRelaxed;
    resultMFCS( vector<Candidate> _MFCS,  vector<Candidate> _MFCSBuffer, long double _theta, long double _thetaRelaxed){
            MFCS = _MFCS;
            MFCSBuffer = _MFCSBuffer;
            theta = _theta;
            thetaRelaxed = _thetaRelaxed;
    }
};

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

resultMFCS topKPeeling(vector<vector<vector<int>>> * graphSamples, vector<AggConComps> *components, int numSamples, int k, long double eps){
    vector<Candidate> tMFCS;
    long double theta = 0;
    long double thetaRelaxed=0;
    vector<Candidate> candidateBuffer;
    IterApriori iApriori = IterApriori(components, numSamples);
    iApriori.setMinSupport(thetaRelaxed);
    int counter = 0;
    while (tMFCS.size() < k){
        vector<int> candidate = iApriori.getNextFrequentItemset();
        counter ++;
        if(counter % 4000 == 0){
        }
        if(candidate.size()>2){
            set<int> candidateSet = convertVectorToSet(candidate);
            double long reliability = subgraphReliability(*graphSamples, &candidateSet, theta);
            if(reliability >= theta){
                tMFCS.push_back(Candidate(candidate, reliability));
            }
        }
    }
    theta = tMFCS[getMinSupportIndex(tMFCS)].support;
    thetaRelaxed = max((long double)0.0,theta-2*eps);
    
    while(iApriori.hasNext()){
        counter++;
        if(counter % 40000 == 0){
        }
        Candidate canCandidate = getNextCandidateAndCheckReliability(iApriori, graphSamples, theta);
        if(canCandidate.nodes.size()==0){
            return resultMFCS(tMFCS, candidateBuffer, theta, thetaRelaxed);
        } 
        if (canCandidate.support > theta){
            replaceLowestReliabilityMFCS(&tMFCS, canCandidate);
            int minIndex = getMinSupportIndex(tMFCS);
            long double minSupport = tMFCS[minIndex].support;
            theta = minSupport;
            thetaRelaxed = max((long double)0.0,theta-2*eps);
            
            iApriori.setMinSupport(thetaRelaxed);
        }
        else if(canCandidate.support > thetaRelaxed){
            candidateBuffer.push_back(canCandidate);
        }
    }
    resultMFCS result = resultMFCS(tMFCS, candidateBuffer, theta, thetaRelaxed);
    return result;
}

resultMFCS runTopKPeelingWithoutSampling(vector<vector<vector<int>>>& samples, int numSamples, int k, long double eps){
    vector<vector<int>> components = connectedComponents(&samples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components, 2);
    int s = filteredComponents.size();
    vector<AggConComps> aggregatedComponents = aggregateConnectedComponents(filteredComponents);
    return topKPeeling(&samples, &aggregatedComponents, numSamples, k, eps);
}

resultMFCS runTopKPeeling(string fileName, int numSamples, int k, long double eps){
    Graph graph(fileName);
    graph.readGraph();
    vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
    return runTopKPeelingWithoutSampling(graphSamples, numSamples, k, eps);
}
#endif





