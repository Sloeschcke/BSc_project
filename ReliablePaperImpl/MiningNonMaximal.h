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

vector<int> getintersectionOfMFCSIDs(vector<int> currentMFCSIDs, int newlyAddedVertex, vector<vector<int>> vertexMFCSIDs){
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

    sort(vector1.begin(), vector1.end());
    sort(vector2.begin(), vector2.end());

    set_difference(vector1.begin(), vector2.end(),
                   vector2.begin(), vector2.end(),
                   back_inserter(diff));

    return diff;
}

vector<int> getVectorFrom1toN(int n){
    vector<int> v( n );
    iota( std::begin( v ), std::end( v ), n );
    return v;
}

bool isVertexSetAnMFCS(vector<int>& vertexSet, vector<vector<int>>& allMFCS){ //MFCS must be sorted already
    sort(vertexSet.begin(), vertexSet.end());
    for (auto mfcs : allMFCS){
        if(vertexSet == mfcs){
            return true;
        }
    }
    return false;
}

vector<vector<int>> sortMFCS(vector<vector<int>> unsorted){
    vector<vector<int>> sorted(unsorted.size());
    for(auto mfcs : unsorted){
        sort(mfcs.begin(), mfcs.end());
        sorted.push_back(mfcs);
    }
    return sorted;
}


vector<vector<int>> runNonMaximal(vector<vector<vector<int>>>& graphSamples, Graph& uncertain, vector<vector<int>>& MFCSs, long double threshold){
    vector<vector<int>> sortedMFCSs = sortMFCS(MFCSs);
    vector<vector<int>> FCS = {};
    vector<vector<int>> allMFCSIDs = vertexMFCSIDs(uncertain, sortedMFCSs);
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
        MiningNonMaximal(graphSamples, chosenNode, vertexSet, allMFCSIDs[chosenNode], con, adjList[chosenNode], &exclusionList, &FCS, allMFCSIDs, threshold, sortedMFCSs, uncertain);
    }
    return FCS;
}


void MiningNonMaximal(vector<vector<vector<int>>>& graphSamples, int chosenNode, vector<int> vertexSet, vector<int> MFCSIDs, vector<bool> con, vector<int> neighbours, vector<int>* exclusionList, vector<vector<int>>* FCS, vector<vector<int>> allMFCSIDs, long double threshold, vector<vector<int>>& MFCS, Graph& uncertain){
    sort(vertexSet.begin(), vertexSet.end());
    for (int i =0; i<graphSamples.size(); i++){
        if (con[i]){
            vector<int> neighbours_v = graphSamples[i][chosenNode];
            sort(neighbours_v.begin(), neighbours_v.end());
            vector<int> neighbours_intersection_vertex_set;
            set_intersection(neighbours_v.begin(), neighbours_v.end(),
                                vertexSet.begin(), vertexSet.end(),
                                back_inserter(neighbours_intersection_vertex_set));
            con[i] = neighbours_intersection_vertex_set.size() != 0; 
        } else {
            set<int> setVertexSet = convertVectorToSet(vertexSet);
            con[i] = isInducedConnectedComponent(graphSamples[i], &setVertexSet);
        }
        int reliability = accumulate(con.begin(), con.end(), 0)/con.size();
        if (reliability > threshold){
            (*FCS).push_back(vertexSet);
        }
    }
    vector<int> difference = getDifferenceOfVectors(neighbours, *exclusionList);
    vertexSet.push_back(chosenNode);
    for (auto & w : difference){
        vector<int> newID = getintersectionOfMFCSIDs(MFCSIDs, w, allMFCSIDs);
        (*exclusionList).push_back(w);
        int newIdSize = newID.size();
        if(newIdSize > 1 || (newIdSize==1 && isVertexSetAnMFCS(vertexSet, MFCS)) ){
            vector<int> neighbours_w = uncertain.adjList[w];
            vector<int> neighboursVW = {};
            set_union(neighbours_w.begin(), neighbours_w.end(), neighbours.begin(), neighbours.end(),back_insert_iterator(neighboursVW));
            MiningNonMaximal(graphSamples, w, vertexSet,  newID,  con, neighboursVW,  exclusionList, FCS, allMFCSIDs,  threshold, MFCS, uncertain);
        }
    }
}

#endif;