#ifndef GRAPH_H
#define GRAPH_H
#include <cassert>
#include "hoved.h"

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
        hasNode = vector<bool> (numNodes);
        adjList = vector<vector<int> > (n);
        probs = vector<vector<double> > (n);
    }
    vector<int> inDeg;

    void readGraph()
    {
        FILE* fin;
        fin = fopen((graph_file).c_str(), "r");
        assert(fin!=NULL);
        int readCnt = 0;
        for (int i = 0; i < numEdges; i++)
        {
            readCnt++;
            int a, b;
            double p;
            int c = fscanf(fin, "%d%d%lf", &a, &b, &p);
            hasNode[a] = true;
            hasNode[b] = true;
            add_edge(a, b, p);
        };
        fclose(fin);
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

