#define HEAD_INFO
#define HEAD_TRACE
#include "hoved.h"
using namespace std;
class Graph
{
public:
    int numNodes, m, k;
    vector<vector<int>> adjMatrix;
    vector<bool> hasNode;
    string graph_file;

    Graph(int n, string fileName) { //Try adding filename with colon here
        numNodes = n;
        m = 9;
        hasNode = vector<bool> (numNodes);
        graph_file = fileName;
        //adjMatrix = vector<vector<int>> v(n, vector<int> (n));
    }
    vector<int> inDeg;

    void readGraph()
    {
        // FILE* fin = fopen((graph_file).c_str(), "r");
        FILE* fin = fopen("graph_file.inf", "r");
        if (fin == NULL){
            cout << "It was null";
        }
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
        int s = 0;
        for (int i = 0; i < numNodes; i++)
            if (hasNode[i])
                s++;
        //INFO(s);
        //ASSERT(readCnt == m);
        fclose(fin);
    }

    void add_edge(int a, int b, double p)
    {
        adjMatrix[a][b] = p;
        adjMatrix[b][a] = p;
    };
};