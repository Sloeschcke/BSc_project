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
    vector<int> newlyAddedMFCSIDs = vertexMFCSIds[newlyAddedVertex];

    sort(currentMFCSIDs.begin(), currentMFCSIDs.end());
    sort(newlyAddedMFCSIDs.begin(), newlyAddedMFCSIDs.end());

    vector<int> intersection;

    set_intersection(currentMFCSIDs.begin(), currentMFcurrentMFCSIDs.end(),
                     newlyAddedMFCSIDs.begin(), newlyAddedMFCSIDs.end(),
                     back_inserter(intersection));
    
    return intersection;
}



vector<vector<int>> runNonMaximal(vector<vector<vector<int>>>& graphSamples, Graph& uncertain, vector<vector<int>>& MFCSs){
    vector<vector<int>> vertexMfcsIDs = vertexMFCSIDs( uncertain, MFCSs);
    
    for (int i=0; i<MFCSs.size(); i++){
        vector<int> exclusionList(MFCSs[i].size()); //allocate size of MFCS
        vector<int> vertexSet(MFCSs[i].size());
        vector<bool> con(uncertain.numNodes, false); 
        for(int j=0;j <MFCSs[i].size(); j++) {
            int vertex = MFCSs[j];
            exclusionList[j]= vertex;
            vertexSet[j] = vertex;
            MiningNonMaximal(graphSamples, vertex, vertexSet, vertexMfcsIDs[vertex], con, uncertain.adjList[vertex], exlusionList);
        }
    
    return {}
}

vector<vector<int>> MiningNonMaximal(vector<vector<vector<int>>>& graphSamples, vector<int> MFCS){
    
    for (int i =0; i<graphSamples.size(), i++){
        if // vertexset is connected in G_i
    }
    return {}
}

#endif