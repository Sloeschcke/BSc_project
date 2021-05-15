import networkx as nx
import matplotlib.pyplot as plt
from networkx.classes.graph import Graph

abspath = "C:\\Users\\mabet\\OneDrive - Aarhus Universitet\\Datalogi\\Bachelor projekt"
_file = "\\BSc_project\\GraphsGeneration\\processed_graphs\\num_nodes\\0\\2.txt"
path_to_graph = abspath+_file # file 0.0

# file_ending = "698"
# r = open(f"processed_graphs/facebook_{file_ending}.edges", "r")

r = open(path_to_graph, "r")

class GraphVisualization:
   
    def __init__(self):
          
        # visual is a list which stores all 
        # the set of edges that constitutes a
        # graph
        self.visual = []
          
    # addEdge function inputs the vertices of an
    # edge and appends it to the visual list
    def addEdge(self, a, b):
        temp = [a, b]
        self.visual.append(temp)
          
    # In visualize function G is an object of
    # class Graph given by networkx G.add_edges_from(visual)
    # creates a graph with a given list
    # nx.draw_networkx(G) - plots the graph
    # plt.show() - displays the graph
    def visualize(self):
        G = nx.Graph()
        G.add_edges_from(self.visual)
        nx.draw_networkx(G)
        plt.show()
    
G = GraphVisualization()
lines = r.readlines()
for line in lines:
    a,b,p = line.split()
    G.addEdge(a,b)
G.visualize()
    