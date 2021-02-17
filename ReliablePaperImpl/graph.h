#define HEAD_INFO
#define HEAD_TRACE
#include <hoved.h>
using namespace std;
class Graph
{
public:
    int numNodes, m, k;
    vector<vector<int>> adjMatrix;
    Graph(int n) {
        numNodes = n;
        //adjMatrix = vector<vector<int>> v(n, vector<int> (n));
    }
    vector<int> inDeg;

    vector<bool> hasnode;
    string graph_file;
    void readGraph()
    {
        FILE* fin = fopen((graph_file).c_str(), "r");
        ASSERT(fin != false);
        int readCnt = 0;
        for (int i = 0; i < m; i++)
        {
            readCnt++;
            int a, b;
            double p;
            int c = fscanf(fin, "%d%d%lf", &a, &b, &p);
            ASSERTT(c == 3, a, b, p, c);

            //TRACE_LINE(a, b);
            ASSERT(a < numNodes);
            ASSERT(b < numNodes);
            hasnode[a] = true;
            hasnode[b] = true;
            add_edge(a, b, p);
        }

        TRACE_LINE_END();
        int s = 0;
        for (int i = 0; i < numNodes; i++)
            if (hasnode[i])
                s++;
        INFO(s);
        ASSERT(readCnt == m);
        fclose(fin);
    }

    void add_edge(int a, int b, double p)
    {
        adjMatrix[a][b] = p;
        adjMatrix[b][a] = p;
    };
};