#include <iostream>
#include "graph.h"
#include "hoved.h"
using namespace std;
void testConnectedComponnets(){
		string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file2.inf";
		std::cout << "Hello ze world";
		int numEdges = 4;
		int numNodes = 6;
		Graph graph (numNodes,numEdges, graph_file);
		graph.readGraph();
	    std::cout << "Started testConnectedComponnets: \n";
	
		graph.connectedComponents();
		std::cout << "Hello ze world";

        cout << "Following are connected components \n";
    }

int main(int argc, char const *argv[])
{
    // string graph_file = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt\\BSc_project\\ReliablePaperImpl\\graph_file.inf";
    // string graph_file = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/ReliablePaperImpl/graph_file.inf";
	// std::cout << "Hello ze world";
	// Graph graph (7, graph_file);
	// graph.readGraph();
	testConnectedComponnets();
	std::cout << "Hello ze world";
	
	return 0;
}

 