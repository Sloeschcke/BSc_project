#include "hoved.h"
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
    //cout << v << " ";
    (*component).insert(v);
    // Recur on all adjacent vertices
    for(auto u : (*sample)[v]){
        if(!(*visited)[u]){
            DFS(u, visited, component, sample);
        }
    }
}

//TODO: flatten to set of set
// Method to print connected components in an undirected graph
set<set<set<int>>> connectedComponents(vector<vector<vector<int>>> *samples)
{
    set<set<set<int>>> setOfComponents;
    for (int i = 0; i < (*samples).size(); i++){
        set<set<int>> components;
        vector<vector<int>> sample = (*samples)[i];
        vector<bool> visited;
        int numNodes = (sample).size();
        // Mark all the vertices as not visited
        visited = vector<bool> (numNodes);
        for (int n = 0; n < numNodes; n++) {
            set<int> component = {};
            if (visited[n] == false) {
                // print all reachable vertices from n
                DFS(n, &visited, &component, &sample);
                //cout << "\n";
                components.insert(component);
            } 
        }
        visited.clear();
        setOfComponents.insert(components);
    }
    return setOfComponents;
}

vector<vector<vector<int>>> sample(Graph g, int number) {
    default_random_engine eng(static_cast<long unsigned int>(time(0)));
    uniform_real_distribution<> distr(0, 1);
    vector<vector<double>> probs = g.probs;
    vector<vector<int>> adjList = g.adjList;
    vector<vector<vector<int>>> samples (number, vector<vector<int>>(adjList.size()));
    for (int n = 0; n < number; n++){
        for(int i = 0 ; i < adjList.size() ; i++) {
            vector<int> vertex = adjList[i];
            for (int j = 0; j < vertex.size() ; j++){
                if (i <= vertex[j]){
                    double prob = distr(eng);
                    if (prob < probs[i][j]){
                        samples[n][i].push_back(vertex[j]);
                        samples[n][vertex[j]].push_back(i);
                    }
                }
            }
        }
    }
    return samples;
}

set<set<int>> prune(set<set<int>> original) {
	bool foundSuperSet;
	for (auto it = original.begin(); it != original.end(); it++){
		foundSuperSet = false;
		for (auto resultElem : original){
			if(*it!=resultElem){
				if(includes(resultElem.begin(), resultElem.end(), it->begin(), it->end())){
					foundSuperSet = true;
				}
			}
		}
		if(foundSuperSet){
			original.erase(it--);
		}
	}
	return original;
}

set<set<int>> apriori(set<set<int>> connectedComponents, double threshold)  {
    //find support > threshold
    // 

}

#endif