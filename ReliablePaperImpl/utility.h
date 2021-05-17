#include "hoved.h"
#include "graph.h"

#ifndef UTILITY_H
#define UTILITY_H

using namespace std;

struct AggConComps {
    int number;
    vector<int> nodes;
    AggConComps(vector<int> _nodes, int _number){
        nodes = _nodes;
        number = _number;
    }
};

vector<AggConComps> aggregateConnectedComponents(vector<vector<int>>& connectedComponents){
    vector<AggConComps> res = {};
    for (auto & comp : connectedComponents){
        sort(comp.begin(), comp.end());
        auto pred = [comp](const AggConComps & item) {
            return item.nodes == comp;
        };
        auto it = find_if(res.begin(), res.end(), pred);
        if(it != res.end()){
            (*it).number++;
        } else {
            res.push_back(AggConComps(comp, 1));
        }
    }
    int scc = connectedComponents.size();
    int sacc = res.size();
    cout << scc << "\n" << sacc << "\n";
    return res;
}

// https://www.geeksforgeeks.org/connected-components-in-an-undirected-graph/ 
// return: sets af sets for hvert CC call
void DFS(int v, vector<bool> *visited, vector<int> *component, vector<vector<int>> *sample)
{
    // Mark the current node as visited and print it
    (*visited)[v] = true;
    // cout << v << " ";
    (*component).push_back(v);
    // Recur on all adjacent vertices
    for(auto &u : (*sample)[v]){
        if(!(*visited)[u]){
            DFS(u, visited, component, sample);
        }
    }
}

vector<vector<int>> removeLenKComponents ( vector<vector<int>> * components, int k){
    vector<vector<int>> res = {};
    for (auto & component : *components){
        if (component.size()>k){
            res.push_back(component);
        }
    }
    return res;
}

// https://www.geeksforgeeks.org/connected-components-in-an-undirected-graph/ 
// Method to print connected components in an undirected graph
vector<vector<int>> connectedComponents(vector<vector<vector<int>>> *samples)
{
    vector<vector<int>> vectorOfComponents;
    for (int i = 0; i < (*samples).size(); i++){
        vector<vector<int>> sample = (*samples)[i];
        vector<bool> visited;
        int numNodes = (sample).size();
        // Mark all the vertices as not visited
        visited = vector<bool> (numNodes);
        for (int n = 0; n < numNodes; n++) {
            vector<int> component;
            if (visited[n] == false) {
                // print all reachable vertices from n
                DFS(n, &visited, &component, &sample);
                // cout << "\n";
                vectorOfComponents.push_back(component);
            }
        }
        visited.clear();
    }
    return vectorOfComponents;
}

void DFSOnSubgraph(int v, vector<bool> *visited, vector<int> * component, vector<vector<int>> *sample, set<int> *subgraph)
{
    // Mark the current node as visited and add to component
    (*visited)[v] = true;
    (*component).push_back(v);
    // Recur on all adjacent vertices
    for(auto & u : (*sample)[v]){
        if((*subgraph).count(u) == 1 && !(*visited)[u]){
            DFSOnSubgraph(u, visited, component, sample, subgraph);
        }
    }
}

// https://www.geeksforgeeks.org/how-to-convert-a-vector-to-set-in-c/
// Function to convert Vector to Set 
set<int> convertVectorToSet(vector<int>& v) { 
    // Declaring the set 
    // using range of vector 
    set<int> s(v.begin(), v.end()); 
    // Return the resultant Set 
    return s; 
} 

vector<int> convertSetToVector(const set<int>& s) {
    vector<int> v(s.begin(), s.end());
    return v;
}

// https://www.geeksforgeeks.org/connected-components-in-an-undirected-graph/ 
// Method to print connected components in an undirected graph
vector<vector<int>> connectedComponentsSubgraph(vector<vector<vector<int>>>& samples, set<int> subgraph)
{
    vector<int> subgraphVector = convertSetToVector(subgraph);
    vector<vector<int>> vectorOfComponents;
    vector<vector<int>> sample;
    for (int i = 0; i < samples.size(); i++){
        sample = samples[i];
        vector<bool> visited;
        int numNodes = (sample).size();
        // Mark all the vertices as not visited
        visited = vector<bool> (numNodes);
        for (auto & node : subgraphVector) {
            vector<int> component;
            if (visited[node] == false) {
                DFSOnSubgraph(node, &visited, &component, &sample, &subgraph);
                vectorOfComponents.push_back(component);
            } 
        }
        visited.clear();
    }
    return vectorOfComponents;
}

bool DFSToCheckConnectivityOfSubgraph(int v, vector<bool> *visited, vector<vector<int>> *sample, const set<int>& subgraph, int counter)
{
    // Mark the current node as visited
    (*visited)[v] = true;
    if(counter==subgraph.size()){
        return true;
    }
    // Recur on all adjacent vertices
    for(auto & u : (*sample)[v]){
        if(subgraph.count(u) == 1 && !(*visited)[u]){
            DFSToCheckConnectivityOfSubgraph(u, visited, sample, subgraph, counter++);
        }
    }
    for(auto & s: subgraph){
        if(!(*visited)[s]) {
            return false;
        }
    }
    return true;
}
vector<vector<vector<int>>> sample(Graph & g, int number) {
    default_random_engine eng(static_cast<long unsigned int>(time(0)));
    // default_random_engine eng(2);
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

vector<vector<int>> pruneVector(vector<vector<int>>& original) {
	bool foundSuperSet;
    vector<vector<int>> res = {};
	for (auto it = original.begin(); it != original.end(); it++){
		foundSuperSet = false;
		for (auto & resultElem : original){
			if(*it!=resultElem){
				if(includes(resultElem.begin(), resultElem.end(), it->begin(), it->end())){
					foundSuperSet = true;
                    break;
				}
			}
		}
		if(!foundSuperSet){
            res.push_back(*it);
        }
	}
	return res;
}

vector<vector<int>> pruneMatrix(vector<vector<vector<int>>>& original){
    int size = original.size();
    if(size <= 3){
        return {};
    }
    vector<vector<int>> res = original[size-1];
    bool foundSuperSet;
    for (int n = original.size()-2; n > 2; n--){
        for(auto it = original[n].begin(); it != original[n].end(); it++){
            foundSuperSet = false;
            for (auto & resultElem : original[n+1]){
                if(includes(resultElem.begin(), resultElem.end(), it->begin(), it->end())){
                    foundSuperSet = true;
                    break;
                }
            }
            if(!foundSuperSet){
                res.push_back(*it);
            }
        }
    }
    return res;
}

set<set<int>> prune(set<set<int>>& original) {
	bool foundSuperSet;
    clock_t start;
    double duration;
    start = clock();

    set<set<int>> res = original;
	for (auto it = original.begin(); it != original.end(); it++){
		foundSuperSet = false;
		for (auto & resultElem : original){
				if(includes(resultElem.begin(), resultElem.end(), it->begin(), it->end())){
                    if(*it!=resultElem){
                        foundSuperSet = true;
                        break;
                    }
				}
			}
		if(foundSuperSet){
			res.erase((*it));
		}
	}
	return res;
}


set<set<int>> vectorVectorToSetSet (vector<vector<int>>& vectorVector){
    set<set<int>> res;
    for(auto & vector:vectorVector){
        res.insert(convertVectorToSet(vector));
    }
    return res;
}

set<set<int>> convertFrequentToSets (vector<vector<vector<int>>>& frequentList){
    set<set<int>> res;
    for (auto & elem1:frequentList){
        for (auto & elem2:elem1){
            res.insert(convertVectorToSet(elem2));
        }
    }
    return res;
}

vector<vector<int>> setSetToVectorVector(set<set<int>>& setSet){
    vector<vector<int>> res;
    for(auto & set:setSet){
        res.push_back(convertSetToVector(set));
    }
    return res;
}

vector<vector<int>> flatten(vector<vector<vector<int>>>& v) {
    std::size_t total_size = 0;
    for (const auto& sub : v)
        total_size += sub.size();
    vector<vector<int>> result;
    result.reserve(total_size);
    for (const auto& sub : v)
        result.insert(result.end(), sub.begin(), sub.end());
    return result;
}

bool isInducedConnectedComponent(vector<vector<int>>& G, const set<int>& subgraph ){
    auto it = subgraph.begin();
    vector<bool> visited(G.size());
    bool containsSubgraph = DFSToCheckConnectivityOfSubgraph(*it, &visited, &G, subgraph, 0);
    return containsSubgraph;
}

double subgraphReliability( vector<vector<vector<int>>>& samples, const set<int>& subgraph, long double threshold){
    double inducedCounter = 0;
    double counter = 0;
    double required = threshold*samples.size();
    for (auto & G: samples){
        bool isInduced = isInducedConnectedComponent(G, subgraph);
        if (isInduced){
            inducedCounter++;
        }
        counter++;
        if (samples.size() - counter < required - inducedCounter){
            break;
        }
    }
    double reliability = inducedCounter/samples.size();
    return reliability;
}

int calculateRequiredSamples(long double epsilon, long double delta, long double numNodes){
	int sampleSize = (1/(2*pow(epsilon,2)))*((numNodes+1)*log(2)-log(delta));
	cout << "sample size: " << sampleSize << "\n";
	return sampleSize;
}


#endif