#ifndef GRAPH_H
#define GRAPH_H
#include <cassert>
#include "hoved.h"

using namespace std;
class Graph
{
public:
    int numNodes, numEdges, k;
    vector<vector<double> > probs;
    vector<vector<int> > adjList;
    vector<bool> hasNode;
    string graph_file;

    Graph(int n, int m, string fileName) : graph_file(fileName){ 
        numNodes = n;
        numEdges = m;
    }
    vector<int> inDeg;

    void readGraph(){

        ifstream fileStream (graph_file);
        assert(fileStream.is_open());
        string problem, value;
        fileStream >> numNodes >> numEdges >> problem >> value;
        hasNode = vector<bool> (numNodes);
        adjList = vector<vector<int> > (numNodes);
        probs = vector<vector<double> > (numEdges);

        string a, b;
        string p;
        while (fileStream >> a >> b >> p){
            int ai = stoi(a);
            int bi = stoi(b);
            hasNode[ai] = true;
            hasNode[bi] = true;
            add_edge(ai, bi, stod(p));
        }
    }
    
    void readGraphfile(ifstream * fileStream){
        string a, b;
        string p;
        while (*fileStream >> a >> b >> p){
            int ai = stoi(a);
            int bi = stoi(b);
            hasNode[ai] = true;
            hasNode[bi] = true;
            add_edge(ai, bi, stod(p));
        }
    }

    void add_edge(int a, int b, double p)
    {
        adjList[a].push_back(b);
        adjList[b].push_back(a);
        probs[a].push_back(p);
        probs[b].push_back(p);
    };
};

#endif

