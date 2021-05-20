#ifndef ITERAPRIORI_H
#define ITERAPRIORI_H
#include "utility.h"
#include "hoved.h"
#include "graph.h"

struct Candidate {
    double long support;
    vector<int> nodes;
    
    Candidate(vector<int> _nodes, double long _support){
        nodes = _nodes;
        support = _support;
    }

    const bool operator < (const Candidate &r) const{
        return (support > r.support);
    }
};

class IterApriori {
private:
    int nowStep;
    long double minSupport;
    vector<AggConComps> transactions;
    vector<vector<int>> C;
    vector<Candidate> nextL;
    vector<Candidate>  L;
    clock_t start;
    double duration;
    long int curr_i;
    long int curr_j;
    int numSamples;
public: 

    IterApriori (vector<AggConComps>* _transactions, int _numSamples) {
        nowStep=0;
        numSamples = _numSamples;
        minSupport = 0;
        curr_i = curr_j = 0;
        transactions = *_transactions;
        generateInitialElements();
    }

    void generateInitialElements(){
            vector<vector<int>> tmp;
            vector<int> element = getElement(transactions);
            for(auto&i:element) tmp.push_back(vector<int>(1, i));
            C=tmp;
    }

    void setMinSupport(long double _minSupport){
        minSupport = _minSupport;
    }

    // inspired by https://github.com/bowbowbow/Apriori/blob/master/apriori.cpp
    long double getSupport(vector<int> *item) {
        int ret = 0;
        for(auto&row:transactions){
            int i, j;
            if(row.nodes.size() < (*item).size()) continue;
            for(i=0, j=0; i < row.nodes.size();i++) {
                if(j==(*item).size()) break;
                if(row.nodes[i] == (*item)[j]) j++;
            }
            if(j==(*item).size()){
                ret += row.number;
            }
        }
        return ((long double)ret)/numSamples;
    }

    bool hasNext(){
        return (L.size()>0) || nowStep==0;
    }

    vector<int> getNextFrequentItemset(){
        //TODO loop
        vector<int> candidate;
        while ( candidate.empty() ||  candidate.size()!=0 ){
            vector<int> candidate = getNextCandidate();
            if(candidate.size()==0 && nowStep!=0){
                //  cout << "Had a zero length candidate";
                 break;
            }
            //TODO check if candidate is used before
            long double support = getSupport(&candidate);
            if(support >= minSupport){
                if(candidate.size() == nowStep+1){
                    nextL.push_back(Candidate(candidate, support));
                    return candidate;
                }
            } 
        } 
        return candidate;
    }

    void cleanLFromRedundant(){
        vector<Candidate> tmp;
        for (auto & candidate : L){
            if(candidate.support >= minSupport){
                tmp.push_back(candidate);
            }
        }
        cout << "Pruned L from size: " << L.size() << " to: " <<tmp.size()<< "\n";
        L = tmp;
    }
    vector<int> getNextCandidate(){
        int redundantCounter = 0;
        int stupidJoin = 0;
        while(L.size()>=1||nowStep==0){
            curr_j++;
            if(nowStep == 0){
                if(curr_j>C.size()){
                    curr_j = curr_i +1;
                    L=nextL;
                    nextL.clear();
                    nowStep++;
                } else{
                    return C[curr_j-1];
                }
            }

            if(curr_j >= L.size()){
                curr_i++;
                curr_j = curr_i+1;
                if (curr_i+1 >= L.size()){
                    cout << "Increasing nowStep: " << nowStep << " having L.size() " << nextL.size() << "\n";
                    nowStep++;
                    L=nextL;
                    if(nextL.size()<1){
                        return vector<int>();
                    }
                    nextL.clear();
                    curr_i = 0;
                    curr_j = curr_i+1;
                }
            }
            if(L[curr_i].support>=minSupport && L[curr_j].support>=minSupport){
                int k;
                for(k=0;k<nowStep-1; k++) {
                    if(L[curr_i].nodes[k] != L[curr_j].nodes[k]) break;
                }
                if(k==nowStep-1){
                    vector<int> tmp (nowStep + 1);
                    int q;
                    for(q=0;q<nowStep-1; q++) {
                        tmp[q] = L[curr_i].nodes[q];
                    }
                    int a = L[curr_i].nodes[nowStep-1];
                    int b = L[curr_j].nodes[nowStep-1];
                    if(a>b) swap(a,b);
                    tmp[q] = a;
                    tmp[q+1] = b;
                    return tmp;
                }
                else {
                    curr_i++;
                    curr_j = curr_i;
                }
            } else {
                redundantCounter++;
                if(redundantCounter%2000 == 0){ 
                    cleanLFromRedundant();
                }
            }
        }
        return vector<int>();
    }
    
    vector<int> getElement(vector<AggConComps> itemset) {
        vector<int> element;
        set<int> s;
        for(auto&row:itemset) for(auto&col:row.nodes) s.insert(col);
        for(auto iter=s.begin(); iter != s.end(); iter++) element.push_back(*iter);
        return element;
    }
};

#endif