#ifndef NONMAXIMAL_H
#define NONMAXIMAL_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "apriori.h"
#include "fastPeeling.h"
#include "TopKPeeling.h"

vector<vector<int>> vertexMFCSIDs(Graph& uncertain, vector<vector<int>>& MFCS){
    vector<vector<int>> uncertainAdjList = uncertain.adjList;
    vector<vector<int>> IDList (uncertainAdjList.size());
    for(int i = 0; i< uncertainAdjList.size(); i++) {
        for(int j = 0 ; j < MFCS.size() ; j++){
            if(find(MFCS[j].begin(), MFCS[j].end(), i) != MFCS[j].end()) {
                IDList[i].push_back(j);
            }
        }
    }
    return IDList;
}

vector<int> getintersectionOfMFCSIDs(vector<int>& currentMFCSIDs, int newlyAddedVertex, vector<vector<int>>& vertexMFCSIDs){
    vector<int> newlyAddedMFCSIDs = vertexMFCSIDs[newlyAddedVertex];

    sort(currentMFCSIDs.begin(), currentMFCSIDs.end());
    sort(newlyAddedMFCSIDs.begin(), newlyAddedMFCSIDs.end());

    vector<int> intersection;

    set_intersection(currentMFCSIDs.begin(), currentMFCSIDs.end(),
                     newlyAddedMFCSIDs.begin(), newlyAddedMFCSIDs.end(),
                     back_inserter(intersection));
    
    return intersection;
}

vector<int> getDifferenceOfVectors(vector<int> vector1, vector<int> vector2){
    vector<int> diff;

    set_difference(vector1.begin(), vector2.end(),
                   vector2.begin(), vector2.end(),
                   back_inserter(diff));

    return diff;
}

vector<int> getVectorFrom1toN(int n){
    vector<int> v( n );
    iota( std::begin( v ), std::end( v ), n );
    return v
}

vector<vector<int>> runNonMaximal(vector<vector<vector<int>>>& graphSamples, Graph& uncertain, vector<vector<int>>& MFCSs){
    vector<vector<int>> FCS;
    vector<vector<int>> allMFCSIDs = vertexMFCSIDs(uncertain, MFCSs);
    int numNodes = uncertain.adjList.size();
    vector<vector<int>> adjList = uncertain.adjList;
    vector<int> nodes = getVectorFrom1toN(numNodes);
    vector<int> exclusionList(numNodes); //allocate size of MFCS
    while (exclusionList.size() < numNodes ){
        vector<int> diff = getDifferenceOfVectors(nodes, exclusionList);
        int chosenNode = diff[0];
        vector<bool> con(graphSamples.size(), false);
        exclusionList.push_back(chosenNode);
        vector<int> vertexSet(numNodes);
        vertexSet[0] = diff[0];
        MiningNonMaximal(graphSamples, chosenNode, vertexSet, allMFCSIDs[chosenNode], con, adjList[chosenNode], &exclusionList, &FCS, allMFCSIDs);
    }
    return FCS;
}

vector<vector<int>> MiningNonMaximal(vector<vector<vector<int>>>& graphSamples, int chosenNode, vector<int> vertexSet, vector<int> MFCSIDs, vector<bool> con, vector<int> neighbours, vector<int>* exclusionList, vector<vector<int>>* FCS, vector<vector<int>> allMFCSIDs){
    for (int i =0; i<graphSamples.size(); i++){
        // vertexset is connected in G_i
    }
    return {}
}



vector<int> vector_uninon (vector<vector<int>>& MFCSs){
     return {} // TODO
}

#endif