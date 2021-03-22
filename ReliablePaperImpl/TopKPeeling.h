// #ifndef TopKPeeling.h
// #define TopKPeeling
// #include "hoved.h"
// #include "graph.h"
// #include "utility.h"
// #include "apriori.h"


// vector<vector<int>> topKPeeling(vector<vector<vector<int>>> * graphSamples, vector<vector<int>> * components){
//     vector<vector<int>> tMFCS;
//     long double theta = 0;
//     IterApriori iApriori = IterApriori(components, theta);
//     iApriori.generateNextC();

    


// }
// vector<vector<int>> runTopKPeeling(string fileName, int numNodes, int numEdges, int numSamples){
//     Graph graph(numNodes, numEdges, fileName);
//     graph.readGraph();
//     vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
//     vector<vector<int>> components = connectedComponents(&graphSamples);
//     return topKPeeling(&graphSamples, &components);
// }

// class IterApriori {
// private:
//     int nowStep;
//     long double minSupport;
//     vector<vector<int> > transactions;
//     vector<vector<int> > C, L;
//     vector<vector<vector<int> > > frequentSet;
//     clock_t start;
//     double duration;
// public: 
//     Apriori (vector<vector<int> > * _transactions, long double _minSupport) {
//         nowStep=0;
//         numSamples = _numSamples;
//         for(auto&row:_transactions){
//             sort(row.begin(), row.end());
//             transactions.push_back(row);
//         }
//         frequentSet.push_back({{}});
//     }


//     vector<vector<int> > generateNextC() {
//         if(nowStep==0) {
//             vector<vector<int> > ret;
//             vector<int> element = getElement(transactions);
//             for(auto&i:element) ret.push_back(vector<int>(1, i));
//             return ret;
//         } else {
//             return pruning(joining());
//         }
//     }
    
//     vector<int> getElement(vector<vector<int> > itemset) {
//         vector<int> element;
//         set<int> s;
//         for(auto&row:itemset) for(auto&col:row) s.insert(col);
//         for(auto iter=s.begin(); iter != s.end(); iter++) element.push_back(*iter);
//         return element;
//     }

// }

// // vector<vector<int>> runTopKPeeling(string fileName, int numNodes, int numEdges, int numSamples, long double threshold){
// //     Graph graph (numNodes, numEdges, fileName);
// // 	graph.readGraph();
// //     vector<vector<vector<int>>> graphSamples =  sample(graph, numSamples);
// //     vector<vector<int>> components = connectedComponents(&graphSamples);
// //     vector<vector<int>> filteredComponents = removeLen1Components(&components);
// //     return topKPeeling(&graphSamples, numSamples);
// // }

// #endif