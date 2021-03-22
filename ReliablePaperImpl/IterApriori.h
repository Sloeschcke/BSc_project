#ifndef ITERAPRIORI_H
#define ITERAPRIORI_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"

struct Candidates {
    int support;
    vector<int> nodes;
};

class IterApriori {
private:
    int nowStep;
    long double minSupport;
    vector<vector<int> > transactions;
    vector<Candidates> C, L;
    vector<vector<Candidates>> frequentSet;
    clock_t start;
    double duration;
public: 

    IterApriori (vector<vector<int> > * _transactions, long double _minSupport) {
        nowStep=0;
        for(auto&row: (*_transactions)){
            sort(row.begin(), row.end());
            transactions.push_back(row);
        }
        frequentSet.push_back({{}});
    }

    vector<vector<int> joining(){
        vector<vector<int>> ret;
        for( int i; i<L.size(); i++){
            if(L[i].support < minSupport) break;
            for(int j; j<L.size(); j++){
                if(L[j].support < minSupport) break;
                int k;
            }
        }
    }
    vector<vector<int> > generateNextC() {
        if(nowStep==0) {
            vector<vector<int> > ret;
            vector<int> element = getElement(transactions);
            for(auto&i:element) ret.push_back(vector<int>(1, i));
            return ret;
        } else {
            return pruning(joining());
        }
    }
    
    vector<int> getElement(vector<vector<int>> itemset) {
        vector<int> element;
        set<int> s;
        for(auto&row:itemset) for(auto&col:row) s.insert(col);
        for(auto iter=s.begin(); iter != s.end(); iter++) element.push_back(*iter);
        return element;
    }
};
#endif