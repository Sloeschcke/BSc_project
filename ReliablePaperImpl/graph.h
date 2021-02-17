#define HEAD_INFO
#define HEAD_TRACE
#include <cassert>
#include "hoved.h"
using namespace std;
class Graph
{
public:
    int numNodes, m, k;
    vector<vector<double>> adjMatrix;
    vector<bool> hasNode;
    string graph_file;

    Graph(int n, string fileName) : graph_file(fileName){ 
        numNodes = n;
        m = 9;
        hasNode = vector<bool> (numNodes);
        adjMatrix = vector<vector<double>> (n, vector<double> (n));
    }
    vector<int> inDeg;

    void readGraph()
    {
        FILE* fin;
        fin = fopen((graph_file).c_str(), "r");
        assert(fin!=NULL);
        int readCnt = 0;
        for (int i = 0; i < m; i++)
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
        adjMatrix[a][b] = p;
        adjMatrix[b][a] = p;
    };
};