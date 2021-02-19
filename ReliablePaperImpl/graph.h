#define HEAD_INFO
#define HEAD_TRACE
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
    vector<bool> visited;
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

            //TRACE_LINE(a, b);
            //ASSERT(b < numNodes);
            hasNode[a] = true;
            hasNode[b] = true;
            add_edge(a, b, p);
        }

        //TRACE_LINE_END();
        //ASSERT(readCnt == m);
        fclose(fin);
    }

    void add_edge(int a, int b, double p)
    {
        adjList[a].push_back(b);
        adjList[b].push_back(a);
        probs[a].push_back(p);
        probs[b].push_back(p);
    };
    
    // Method to print connected components in an undirected graph
    void connectedComponents()
    {
        // Mark all the vertices as not visited
        visited = vector<bool> (numNodes);
        
        for (int n = 0; n < numNodes; n++) {
            if (visited[n] == false) {
                // print all reachable vertices from n
                DFS(n, &visited);
                cout << "\n";
            }
        }
        visited.clear();
    }
    
    void DFS(int v, vector<bool> *visited)
    {
        // Mark the current node as visited and print it
        (*visited)[v] = true;
        cout << v << " ";
    
        // Recur on all adjacent vertices
        for(auto u : adjList[v]){
            if(!(*visited)[u]){
                DFS(u, visited);
            }
        }
    }

   
};

