#include "hoved.h"
#include "sampler.h"
#include "graph.h"

#ifndef UTILITY_H
#define UTILITY_H

using namespace std;

//loop over samples og kald CC på hver
// return: sets af sets for hvert CC call

void DFS(int v, vector<bool> *visited, set<int> *component, vector<vector<int>> *sample)
{
    // Mark the current node as visited and print it
    (*visited)[v] = true;
    cout << v << " ";
    (*component).insert(v);
    // Recur on all adjacent vertices
    for(auto u : (*sample)[v]){
        if(!(*visited)[u]){
            DFS(u, visited, component, sample);
        }
    }
}

// Method to print connected components in an undirected graph
set<set<int>> connectedComponents(vector<vector<int>> *sample)
{
    set<set<int>> components;
    vector<bool> visited;
    int numNodes = (*sample).size();
    // Mark all the vertices as not visited
    visited = vector<bool> (numNodes);
    for (int n = 0; n < numNodes; n++) {
        set<int> component = {};
        if (visited[n] == false) {
            // print all reachable vertices from n
            DFS(n, &visited, &component, sample);
            cout << "\n";
            components.insert(component);
        }
    }
    visited.clear();
    //more memory leak??
    return components;
}

#endif