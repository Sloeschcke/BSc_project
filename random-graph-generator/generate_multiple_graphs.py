
import os
import random
from pathlib import Path


def main():
    # generate_edge_degree_graphs()
    generate_numNodes_graphs()
    
def get_relative_path(value_for_plot):
    relative_path = Path("C:/Users/mabet/OneDrive - Aarhus Universitet/Datalogi/Bachelor projekt/BSc_project/GraphsGeneration/processed_graphs")
    print(os.path)
    path = relative_path/value_for_plot
    if not os.path.exists(path):
        os.makedirs(path)
    return path

def generate_numNodes_graphs():
    num_nodes = [10, 20, 40, 80, 160, 280, 560]
    edge_degree = 2
    value_for_plot = "num_nodes_edgedegree2"
    path = get_relative_path(value_for_plot)
    for i, nodes in enumerate(num_nodes):
        edges = edge_degree*nodes
        create_processed_graph_file(path, i, nodes, edges, 1, value_for_plot, nodes)

    
def generate_edge_degree_graphs():
    value_for_plot = "edge_degree"
    relative_path_processed = get_relative_path(value_for_plot)
    nodes = 80
    edges = [80,120,160,200,240,280,320]
    for index,i in enumerate(edges):
        value = i/nodes
        create_processed_graph_file(relative_path_processed, index, nodes, i, 1, value_for_plot, value)

def create_processed_graph_file(path, index, nodes, edges, seed, value_for_plot,value):
    filename = str(index)
    create_graph_file( nodes, edges, filename, seed=seed)
    
    r = open(filename, "r")
    first_line = r.readline()
    
    filename_processed = path/f"{str(index)}.txt"
    w = open(filename_processed, "w+")
    
    lines = r.readlines()

    w.write(first_line.rstrip("\n") +" "+value_for_plot + " " + str(value)+"\n")
    for nodes in lines:
        editedLine = f"{nodes.rstrip()} {str(random.uniform(0,1))}\n"
        w.write(editedLine)

    w.close()
    r.close()
    os.remove(filename)

def create_graph_file( nodes, edges, filename , seed):
    command_nodes_and_edges = "python gen_graph.py -grnm -n " + str(nodes) + " -m " + str(edges) + " --seed "+ str(seed) + " --out " + filename
    os.system(command_nodes_and_edges)

if __name__ == "__main__":
    main()