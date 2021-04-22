#ifndef NONMAXIMAL_H
#define NONMAXIMAL_H
#include "hoved.h"
#include "graph.h"
#include "utility.h"
#include "apriori.h"
#include "fastPeeling.h"
#include "TopKPeeling.h"

void print(std::vector<int> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ' ';
    }
}

vector<int> vector_union(vector<int> v1, vector<int> v2){
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    vector<int> v3 = {};
    set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));
    return v3;
}
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

vector<int> getMFCSNodes(vector<vector<int>> MFCS){
    vector<int> res = {};
    for(auto mfcs : MFCS){
        res = vector_union(res, mfcs);
    }
    return res;
}

bool doesVectorContainVector(vector<int> v1, vector<int> v2){
    for(int i = 0; i<v2.size(); i++){
        bool isV2EleInV1 = find(v1.begin(), v1.end(), v2[i]) != v1.end();
        if(!isV2EleInV1){
            return false;
        }
    }
    return true;
}

vector<int> getDifferenceOfVectors(vector<int> vector1, vector<int> vector2){
    if(vector1.size() == 0){
        return vector2;
    }
    if(vector2.size() == 0){
        return vector1;
    }

    vector<int> diff = {};

    sort(vector1.begin(), vector1.end());
    sort(vector2.begin(), vector2.end());

    set_difference(vector1.begin(), vector1.end(),
                   vector2.begin(), vector2.end(),
                   back_inserter(diff));

    return diff;
}

vector<int> getVectorFrom1toN(int n){
    vector<int> v(n);
    iota(begin(v), end(v), 0);
    return v;
}

bool isVertexSetAnMFCS(vector<int>& vertexSet, vector<vector<int>>& allMFCS){ //MFCS must be sorted already
    for (auto mfcs : allMFCS){
        if(vertexSet == mfcs){
            return true;
        }
    }
    return false;
}

vector<vector<int>> sortMFCS(vector<vector<int>> unsorted){
    vector<vector<int>> sorted = {};
    for(auto mfcs : unsorted){
        sort(mfcs.begin(), mfcs.end());
        sorted.push_back(mfcs);
    }
    return sorted;
}

void MiningNonMaximal(vector<vector<vector<int>>>& graphSamples, int chosenNode, vector<int> vertexSet, vector<int> MFCSIDs, vector<bool> con, vector<int> neighbours, vector<int>* exclusionList, vector<NodesAndReliability>* FCS, vector<vector<int>>& allMFCSIDs, long double threshold, vector<vector<int>>& MFCS, Graph& uncertain){
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
    }
    long double reliability = static_cast<double>(accumulate(con.begin(), con.end(), 0))/con.size();
    if (reliability > threshold){
        NodesAndReliability FCS_ele = NodesAndReliability(vertexSet, reliability);
        if(FCS_ele.nodes.size() >= 3){
            (*FCS).push_back(FCS_ele);
        }
    }
    if (count(vertexSet.begin(), vertexSet.end(), chosenNode) == 0) {
        vertexSet.push_back(chosenNode);
    }

    vector<int> difference = getDifferenceOfVectors(neighbours, *exclusionList);
    
    for (auto & w : difference){
        vector<int> newID = getintersectionOfMFCSIDs(MFCSIDs, w, allMFCSIDs);
        (*exclusionList).push_back(w);
        int newIdSize = newID.size();
        if(newIdSize > 1 || (newIdSize==1 && !isVertexSetAnMFCS(vertexSet, MFCS)) ){
            vector<int> neighbours_w = uncertain.adjList[w];
            vector<int> u = vector_union(neighbours, neighbours_w);
            MiningNonMaximal(graphSamples, w, vertexSet,  newID,  con, u,  exclusionList, FCS, allMFCSIDs,  threshold, MFCS, uncertain);
        }
    }
}

vector<NodesAndReliability> runNonMaximal(vector<vector<vector<int>>>& graphSamples, Graph& uncertain, vector<vector<int>>& MFCSs, long double threshold){
    vector<vector<int>> sortedMFCSs = sortMFCS(MFCSs);
    vector<int> nodes = getMFCSNodes(sortedMFCSs);
    vector<NodesAndReliability> FCS = {};
    vector<vector<int>> allMFCSIDs = vertexMFCSIDs(uncertain, sortedMFCSs);
    int numNodes = uncertain.adjList.size();
    vector<vector<int>> adjList = uncertain.adjList;
    // vector<int> nodes = getVectorFrom1toN(numNodes);
    vector<int> exclusionList = {};
    bool finished = false;
    while (!finished){
        vector<int> diff = getDifferenceOfVectors(nodes, exclusionList);
        int chosenNode = diff[0];
        vector<bool> con(graphSamples.size(), false);
        exclusionList.push_back(chosenNode);
        vector<int> vertexSet = {};
        vertexSet.push_back(chosenNode);
        MiningNonMaximal(graphSamples, chosenNode, vertexSet, allMFCSIDs[chosenNode], con, adjList[chosenNode], &exclusionList, &FCS, allMFCSIDs, threshold, sortedMFCSs, uncertain);
        finished = doesVectorContainVector(exclusionList, nodes);
    }
    return FCS;
}


// vector<NodesAndReliability> runNonMaximal(vector<vector<vector<int>>>& graphSamples, Graph& uncertain, vector<vector<int>>& MFCSs, long double threshold){
//     vector<vector<int>> sortedMFCSs = sortMFCS(MFCSs);
//     vector<NodesAndReliability> FCS = {};
//     vector<vector<int>> allMFCSIDs = vertexMFCSIDs(uncertain, sortedMFCSs);
//     int numNodes = uncertain.adjList.size();
//     vector<vector<int>> adjList = uncertain.adjList;
//     vector<int> nodes = getVectorFrom1toN(numNodes);
//     vector<int> exclusionList = {}; 
//     while (exclusionList.size() < numNodes ){
//         vector<int> diff = getDifferenceOfVectors(nodes, exclusionList);
//         int chosenNode = diff[0];
//         vector<bool> con(graphSamples.size(), false);
//         exclusionList.push_back(chosenNode);
//         vector<int> vertexSet = {};
//         vertexSet.push_back(chosenNode);
//         MiningNonMaximal(graphSamples, chosenNode, vertexSet, allMFCSIDs[chosenNode], con, adjList[chosenNode], &exclusionList, &FCS, allMFCSIDs, threshold, sortedMFCSs, uncertain);
//     }
//     return FCS;
// }

#endif