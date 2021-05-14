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

vector<Candidate> pruneCandidateBuffer(vector<Candidate> & candidateBuffer, long double thetaRelaxed){
    vector<Candidate> resSet;
    for(auto &cand: candidateBuffer){
        if(cand.support >= thetaRelaxed){
            resSet.push_back(cand);
        }
    }
    return resSet;
} 

Candidate replaceLowestReliabilityMFCS(vector<Candidate>* tMFCS, Candidate& replacee){
    int minIndex = getMinSupportIndex(*tMFCS);
    Candidate result = (*tMFCS)[minIndex];
    if(replacee.support > (*tMFCS)[minIndex].support){
        (*tMFCS)[minIndex] = replacee;
    }
    return result;
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
            Candidate replacedElem = replaceLowestReliabilityMFCS(&tMFCS, canCandidate);
            int minIndex = getMinSupportIndex(tMFCS);
            long double minSupport = tMFCS[minIndex].support;
            theta = minSupport;
            thetaRelaxed = max((long double)0.0,theta-2*eps);
            cout <<"ThetaRelaxed: " << thetaRelaxed<<"\n";
            cout <<"Theta: " << theta<<"\n"; 
            iApriori.setMinSupport(thetaRelaxed);
            if(replacedElem.support >= thetaRelaxed){
                candidateBuffer.push_back(replacedElem);
            }
            candidateBuffer = pruneCandidateBuffer(candidateBuffer, thetaRelaxed);
        }
        else if(canCandidate.support >= thetaRelaxed){
            candidateBuffer.push_back(canCandidate);
        }
    }
    resultMFCS result = resultMFCS(tMFCS, candidateBuffer, theta, thetaRelaxed);
    return result;
}




long double calculateEpsilon(long double delta, long double numSamples, int CandidateSetSize ){
    return sqrt(log((2*CandidateSetSize)/delta)/(2.0*numSamples)); 
}


vector<Candidate> updateReliabilities(vector<vector<vector<int>>> &graphSamples, vector<Candidate> & candidates, int numSampled){
    for (auto &cand : candidates){
        set<int> candidate = convertVectorToSet(cand.nodes);
        double long reliability = subgraphReliability( graphSamples, &candidate, 0);
        double long newSupport = (cand.support * numSampled + graphSamples.size() * reliability)/(numSampled + graphSamples.size());
        cand.support = newSupport;
    }
    return candidates ;
}

vector<Candidate> filterCandidates(vector<Candidate> & candidates, int k, double long epsilon){
    sort(candidates.begin(), candidates.end());
    long double kReliability = candidates[k-1].support;
    vector<Candidate> tmp;
    
    //get min dinstance bethween kth most reliable candidate and all candidates with reliability < k
    for (auto & candidate : candidates){
        if(candidate.support>=kReliability-2*epsilon){
            tmp.push_back(candidate);
        }
    }
    candidates = tmp;
    return candidates;
}

vector<Candidate> topKPeelingStep2 (Graph & graph, resultMFCS & step1Results, int stepSize, long double epsilon, long double delta, int k, int step1NumSamples, long double epsilonLimit){
    int numSampled = step1NumSamples;
    int resultSize = step1Results.MFCS.size() + step1Results.MFCSBuffer.size();
    int numSamples = stepSize;
    vector<Candidate> candidates = step1Results.MFCS;
    candidates.insert(candidates.end(), step1Results.MFCSBuffer.begin(),  step1Results.MFCSBuffer.end() ); 
    long double kth_reliability = 0;
    while(numSampled < 100000000 && candidates.size()>k){
        vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
        candidates = updateReliabilities(graphSamples,  candidates, numSampled);
        numSampled = numSampled+numSamples;
        cout << numSampled << "\n";
        long double currentEpsilon = calculateEpsilon(delta, numSampled, resultSize);
        if(currentEpsilon<epsilonLimit) {
            cout << "Early stopped";
            sort(candidates.begin(), candidates.end());
            vector<Candidate> candidates_topk;
            copy(candidates.begin(), candidates.begin()+k+1, candidates_topk.begin());
            return candidates_topk;
        }
        candidates = filterCandidates(candidates, k, currentEpsilon);
    }
    return candidates;
} 




resultMFCS runTopKPeelingWithoutSampling(vector<vector<vector<int>>>& samples, int numSamples, int k, long double eps){
    vector<vector<int>> components = connectedComponents(&samples);
    vector<vector<int>> filteredComponents = removeLenKComponents(&components, 2);
    int s = filteredComponents.size();
    vector<AggConComps> aggregatedComponents = aggregateConnectedComponents(filteredComponents);
    return topKPeeling(&samples, &aggregatedComponents, numSamples, k, eps);
}

resultMFCS runTopKPeeling(string fileName, int k, long double eps, long double delta){
    Graph graph(fileName);
    double long numCombinations = pow(2, graph.numNodes +1);
    //graph.readGraph();
    int numSamples = calculateRequiredSamples(0.01, eps, pow(2, graph.numNodes +1));

    vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
    resultMFCS step1 = runTopKPeelingWithoutSampling(graphSamples, numSamples, k, eps);
    vector<Candidate> step2 = topKPeelingStep2 (graph, step1, 10000, eps, delta, k, numSamples, 0.001);
    resultMFCS result = resultMFCS(step2, step1.MFCSBuffer, step1.theta, step1.thetaRelaxed);
    return result; //should be step2
}
#endif





