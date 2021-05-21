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
    string valueForPlot;

    Graph(string fileName) : graph_file(fileName){ 
        readGraph();
    }
    vector<int> inDeg;

    void readGraph(){
        ifstream fileStream (graph_file);
        cout << graph_file;
        assert(fileStream.is_open());
        string problem, value, numNodesStream, numEdgesStream;
        fileStream >> numNodesStream >> numEdgesStream >> problem >> value;
        valueForPlot = value;
        numNodes = stod(numNodesStream);
        numEdges = stod(numEdgesStream);
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

    string getValue(){
        return valueForPlot;
    }

    int getNumNodes(){
        return numNodes;
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

