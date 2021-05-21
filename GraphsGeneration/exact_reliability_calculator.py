
from pathlib import Path
import networkx as nx
from itertools import combinations
from operator import itemgetter

abspath = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt"
_file = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\Datalogi\\Bachelor projekt\\BSc_project\\GraphsGeneration\\processed_graphs\\ToyGraph\\graph_file5.inf"
# path_to_graph_reliable_paper = Path(abspath+_file) # file 0.0
# path_to_graph_reliable_paper = Path("C:/Users/chris/Documents/6. Semester/Bachelor Project/BSc_project/ReliablePaperImpl/test_graphs/graph_file5.inf")

# 1, 4, 9, with reliability: 0.927607
# 1, 4, 9, with reliability: 0.910838
# 1, 2, 4, with reliability: 0.938241
def main():
    edge_set = get_edgeSet(_file)
    worlds = get_possible_worlds(edge_set)
    world_probs = get_world_probabilities(worlds, edge_set)
    
    combinations = get_all_node_combinations(7)
    print(len(combinations))
    topk = get_topk_reliabilities(worlds, world_probs, 5, combinations, 7)
    print(topk)
    # reliability = get_reliability(worlds, world_probs, subgraph_to_check, 7)
    # print(reliability)

def get_edgeSet(path_to_graph):
    r = open(path_to_graph, "r")
    lines = r.readlines()

    edges_set = set()

    for line in lines[1:]:
        numbers = [n for n in line.split()]
        edges_set.add(tuple([int(numbers[0]), int(numbers[1]), float(numbers[2])]))
    return edges_set  

def get_possible_worlds(edge_set):
    edge_combinations = list()
    for r in range(len(edge_set)):
        edge_combinations = edge_combinations + list(combinations(edge_set, r+1))

    return edge_combinations

def get_all_node_combinations(numNodes):
    nodes = list(range(0,numNodes))
    node_combinations = list()
    for n in nodes:
        node_combinations = node_combinations + list(combinations(nodes, n+1))
    return node_combinations


def get_world_probabilities(worlds, edge_set):
    probs = list()
    for world in worlds:
        prob = 1
        for edge in edge_set:
            if(edge in world):
                prob = prob * edge[2]
            else:
                prob = prob * (1 - edge[2])
        probs.append(prob)
    return probs

def get_reliability(worlds, probabilities, subgraph, numNodes):
    reliability = 0
    counter = 0
    for world in worlds:
        nodes = list(range(0, numNodes))
        G = nx.Graph()
        G.add_nodes_from(nodes)
        for edge in world:
            G.add_edge(edge[0], edge[1])
        nxsubgraph = G.subgraph(subgraph)
        if(nx.is_connected(nxsubgraph)):
            reliability += probabilities[counter]
        counter += 1
    
    return reliability

def get_topk_reliabilities(worlds, probabilities, k, components, numNodes):
    reliabilities = list()
    for c in components:
        if(len(c) > 2):
            reliabilities = reliabilities + [(c, get_reliability(worlds, probabilities, c, numNodes))]
        # print(reliabilities)
    sorted_reliabilities = sorted(reliabilities, key = itemgetter(1))
    return sorted_reliabilities[-k: ]

main()