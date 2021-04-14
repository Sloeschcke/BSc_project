#ifndef ITERAPRIORI_H
#define ITERAPRIORI_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"

struct Candidate {
    double long support;
    vector<int> nodes;
    Candidate(vector<int> _nodes, double long _support){
        nodes = _nodes;
        support = _support;
    }
};

class IterApriori {
private:
    int nowStep;
    long double minSupport;
    vector<vector<int> > transactions;
    vector<vector<int>> C;
    vector<Candidate> nextL;
    vector<Candidate>  L;
    clock_t start;
    double duration;
    long int curr_i;
    long int curr_j;
    int numSamples;
public: 

    IterApriori (vector<vector<int> > * _transactions, int _numSamples) {
        nowStep=0;
        numSamples = _numSamples;
        minSupport = 0;
        curr_i = curr_j = 0;
        for(auto&row: (*_transactions)){
            sort(row.begin(), row.end()); // TODO find out why - similarity reasons
            transactions.push_back(row);
        }
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
            if(row.size() < (*item).size()) continue;
            for(i=0, j=0; i < row.size();i++) {
                if(j==(*item).size()) break;
                if(row[i] == (*item)[j]) j++;
            }
            if(j==(*item).size()){
                ret++;
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
                 cout << "Had a zero length candidate";
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


    vector<int> getNextCandidate(){
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
            // // vector<Candidate> copyL = L;
            // if(lsize != L.size()){
            //     lsize = L.size();
            //     cout << "size \n" << lsize ;
            // }
            if(curr_j >= L.size()){
                curr_i++;
                curr_j = curr_i+1;
                if (curr_i+1 >= L.size()){
                    cout << "Increasing nowStep\n";
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
                    vector<int> tmp;
                    for(int k=0;k<nowStep-1; k++) {
                        tmp.push_back(L[curr_i].nodes[k]);
                    }
                    int a = L[curr_i].nodes[nowStep-1];
                    int b = L[curr_j].nodes[nowStep-1];
                    if(a>b) swap(a,b);
                    tmp.push_back(a), tmp.push_back(b);
                    // ret.push_back(tmp);
                    return tmp;
                }
            }
        }
        return vector<int>();
    }

    // vector<vector<int> > generateNextC() {
    //     if(nowStep==0) {
    //         vector<vector<int> > ret;
    //         vector<int> element = getElement(transactions);
    //         for(auto&i:element) ret.push_back(vector<int>(1, i));
    //         return ret;
    //     } else {
    //         return pruning(joining());
    //     }
    // }
    
    vector<int> getElement(vector<vector<int>> itemset) {
        vector<int> element;
        set<int> s;
        for(auto&row:itemset) for(auto&col:row) s.insert(col);
        for(auto iter=s.begin(); iter != s.end(); iter++) element.push_back(*iter);
        return element;
    }
};

#endif