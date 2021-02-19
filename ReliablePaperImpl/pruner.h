#ifndef PRUNER_H
#define PRUNER_H
#include "hoved.h"


using namespace std;

set<set<int>> prune(set<set<int>> original) {
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
#endif


