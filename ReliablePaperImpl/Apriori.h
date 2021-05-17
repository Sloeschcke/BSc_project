#ifndef APRIORI_H
#define APRIORI_H
#include "hoved.h"

using namespace std;

long double round(long double value, int pos){
    long double temp;
    temp = value * pow( 10, pos );
    temp = floor( temp + 0.5 );
    temp *= pow( 10, -pos );
    return temp;
}

//Copied from
//https://github.com/bowbowbow/Apriori/blob/master/apriori.cpp
class Apriori {
private:
    int nowStep;
    long double minSupport;
    vector<vector<int> > transactions;
    vector<vector<int> > C, L;
    vector<vector<vector<int> > > frequentSet;
    int numSamples;
    clock_t start;
    double duration;
public:
    
    Apriori (vector<vector<int>> &_transactions, long double _minSupport, int _numSamples) {
        nowStep=0;
        numSamples = _numSamples;
        minSupport = _minSupport;
        for(auto&row:_transactions){
            sort(row.begin(), row.end());
            transactions.push_back(row);
        }
        frequentSet.push_back({{}});
    }
    vector<vector<vector<int> > > getFrequentSet(){
        return frequentSet;
    }

    void process() {
            while(true) {
                C = generateNextC();
                int C_size = C.size();
                if(C.size()==0) break;
                nowStep++;
                
                L = generateL();
                frequentSet.push_back(L);
            }
    }

    vector<int> getElement(vector<vector<int>> &itemset) {
        vector<int> element;
        set<int> s;
        for(auto&row:itemset) for(auto&col:row) s.insert(col);
        for(auto iter=s.begin(); iter != s.end(); iter++) element.push_back(*iter);
        return element;
    }
    
    vector<vector<int> > generateNextC() {
        if(nowStep==0) {
            vector<vector<int> > ret;
            vector<int> element = getElement(transactions);
            for(auto&i:element) ret.push_back(vector<int>(1, i));
            
            return ret;
        } else {
            auto joined = joining();
            return pruning(joined);
        }
    }
    
    vector<vector<int>> joining () {
        start = clock();
        vector<vector<int> > ret;
        for(int i=0;i<L.size();i++){
            for(int j=i+1;j<L.size(); j++) {
                int k;
                for(k=0;k<nowStep-1; k++) {
                    if(L[i][k] != L[j][k]) break;
                }
                if(k == nowStep-1) {
                    vector<int> tmp(nowStep + 1);
                    int q;
                    for(q=0; q<nowStep-1; q++) {
                        tmp[q] = L[i][q];
                    }
                    int a = L[i][nowStep-1];
                    int b = L[j][nowStep-1];
                    if(a>b) swap(a,b);
                    tmp[q] = a;
                    tmp[q+1] = b;
                    ret.push_back(tmp);
                }
            }
        }
        duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
        return ret;
    }
    
    vector<vector<int>> pruning (vector<vector<int>> & joined) {
        start = clock();

        vector<vector<int> > ret;
        
        set<vector<int> > lSet;
        for(auto&row:L) lSet.insert(row);
        
        for(auto&row:joined){
            int i;
            for(i=0;i<row.size();i++){
                vector<int> tmp = row;
                tmp.erase(tmp.begin()+i);
                if(lSet.find(tmp) == lSet.end()) {
                    break;
                }
            }
            if(i==row.size()){
                ret.push_back(row);
            }
        }
        duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
        return ret;
    }
    
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
    
    vector<vector<int> > generateL() {
        start = clock();
        vector<vector<int> > ret;
        for(auto&row:C){
            vector<int> problem = {0,2,3,4,5,8,9}; 
            if(row == problem){
                cout << "problematic row" << "\n";
            }
            long double support = getSupport(&row);
            if(support < minSupport) continue;
            ret.push_back(row);
        }
        duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

        return ret;
    }
};



set<set<int>> getMFI(vector<vector<int>> & components, double threshold, int numSamples){
    Apriori apriori (components, threshold, numSamples);
    apriori.process();
    vector<vector<vector<int>>> result = apriori.getFrequentSet();
    
    vector<vector<int>> MFI = pruneMatrix(result);
    set<set<int>> setMFI = vectorVectorToSetSet(MFI);
    return setMFI; 

}
#endif