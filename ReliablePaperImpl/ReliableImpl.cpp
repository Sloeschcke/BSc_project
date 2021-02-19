#include <iostream>
#include "graph.h"
#include "hoved.h"
#include "pruner.h"
#include "tests.h"

using namespace std;

// // string GetPath2()
// // {
// //      wchar_t buffer[MAX_PATH] = {};
// //      ::GetSystemDirectoryA(buffer,_countof(buffer)); // notice the A
// //      strcat(buffer,"\\version.dll");

// //      return std::string(buffer);
// //  }
// //  #include <string>
// // #include <windows.h>

// std::string getexepath()
// {
//   char result[ MAX_PATH ]={};
//   return std::string( result, GetCurrentDirectoryA( MAX_PATH, result) );
// }

int main(int argc, char const *argv[])
{
    // string graph_file = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt\\BSc_project\\ReliablePaperImpl\\graph_file.inf";
    // string graph_file = +"\\ReliablePaperImpl\\graph_file.inf";
	std::cout << "Hello ze world";
	// string c = getexepath();
	string graph_file = ".\\ReliablePaperImpl\\graph_file.inf";
	Graph graph (7, graph_file);
	graph.readGraph();
	std::cout << "Hello ze world";
	testAll();
	return 0;
}

