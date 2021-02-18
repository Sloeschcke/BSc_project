#include <iostream>
#include "graph.h"
#include "hoved.h"
using namespace std;

set<set<int>> prune(set<set<int>> original) {
	set<set<int>> result = original;
//   while (it!=original.end()){
//   	set<set<int>>::iterator itResult = result.begin();
// 	while (itResult!=result.end()){
// 		set_difference(*it.begin(), B.end(), A.begin(), A.end(), inserter(C, C.begin()));
		
// 		itResult++;
// 	}
// 	it++;
//   }
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

void testPrune(){
	set<set<int>> vertices = {{1,2,3}, {1,2}, {1,4}};

	set<set<int>> expected = {{1,2,3}, {1,4}};
	assert(prune(vertices)==expected);
}

int main(int argc, char const *argv[])
{
    // string graph_file = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt\\BSc_project\\ReliablePaperImpl\\graph_file.inf";
	// std::cout << "Hello ze world";
	// Graph graph (7, graph_file);
	// graph.readGraph();
	// std::cout << "Hello ze world";
	testPrune();
	return 0;
}