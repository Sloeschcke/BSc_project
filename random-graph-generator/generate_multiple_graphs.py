from scipy.stats import f, norm
import os
import random
import shutil
from pathlib import Path
import numpy as np


def main():
    valency_probs = [0.1, 0.3, 0.5, 0.7, 0.9]
    # generate_edge_degree_graphs()
    generate_numNodes_graphs_f_dist()
    # generate_edge_degree_graphs(valency= valency_probs)
    # generate_numNodes_graphs()
    # generate_numNodes_graphs(valency = valency_probs)
    # generate_data_for_vary_k()
    # generate_data_for_vary_k(valency = valency_probs)

    
def get_relative_path(value_for_plot):
    relative_path = Path("C:/Users/mabet/OneDrive - Aarhus Universitet/Datalogi/Bachelor projekt/BSc_project/GraphsGeneration/processed_graphs")
    # relative_path = Path("C:/Users/chris/Documents/6. Semester/Bachelor Project/BSc_project/GraphsGeneration/processed_graphs")

    print(os.path)
    path = relative_path/value_for_plot
    if not os.path.exists(path):
        os.makedirs(path)
    outputPath = path/"output"
    if os.path.exists(outputPath):
        shutil.rmtree(outputPath, ignore_errors=True)
    if not os.path.exists(outputPath):
        os.makedirs(outputPath)
    return path

def generate_numNodes_graphs(valency = None):
    num_nodes = [10,15,20,25,30,35,40,45,50,55]
    edge_degree = 2
    num_samples = 5
    value_for_plot = "num_nodes"
    path = get_relative_path(value_for_plot)
    for folder_index, nodes in enumerate(num_nodes):
        for sample_index in range(num_samples):
            edges = edge_degree*nodes
            create_processed_graph_files(path, folder_index, sample_index, nodes, edges, 1, value_for_plot, nodes, valency)

def generate_numNodes_graphs_f_dist():
    num_nodes = [50, 100, 200, 400, 800]
    edge_degree = 2
    num_samples = 5
    value_for_plot = "num_nodes_f_dist"
    path = get_relative_path(value_for_plot)
    for folder_index, nodes in enumerate(num_nodes):
        for sample_index in range(num_samples):
            edges = edge_degree*nodes
            create_f_graph(path, folder_index, sample_index, nodes, edges, 1, value_for_plot, nodes, None)

    
def generate_edge_degree_graphs(valency = None):
    value_for_plot = "edge_degree"
    path = get_relative_path(value_for_plot)
    num_samples = 5
    nodes = 30
    edges = [30,36,42,48,54,60,66,72,78,84,90]
    for folder_index, edges in enumerate(edges):
        for sample_index in range(num_samples):
            value = edges/nodes
            create_processed_graph_files(path, folder_index, sample_index, nodes, edges, 1, value_for_plot, value, valency)

def generate_data_for_vary_k(valency = None):
    value_for_plot = "Varying_K"
    relative_path_processed = get_relative_path(value_for_plot)
    nodes = 30
    edges = 2*nodes
    numSamples = 5
    k = 10
    for folder_index in range(k):
        for sample_index in range(numSamples):
            value = k + 1
            create_processed_graph_files(relative_path_processed, folder_index, sample_index, nodes, edges, 1, value_for_plot, value, valency)
        
def create_processed_graph_files(path, folderIndex, index, nodes, edges, seed, value_for_plot, value, valency = None):
    subFolder = path/str(folderIndex)
    if(valency is None):
        subFolder = path/str(folderIndex)
    else:
        subFolder = path/(str(folderIndex) + "_valency")
    if not os.path.exists(subFolder):
        os.mkdir(subFolder)
    filename = str(index)
    create_graph_file(nodes, edges, filename, seed=seed+index)
    
    r = open(filename, "r")
    first_line = r.readline()
    
    filename_processed = subFolder/f"{str(index)}.txt"

    w = open(filename_processed, "w+")
    
    lines = r.readlines()

    if(valency is None):
        w.write(first_line.rstrip("\n") +" "+value_for_plot + " " + str(value)+"\n")
        for nodes in lines:
            editedLine = f"{nodes.rstrip()} {str(random.uniform(0,1))}\n"
            w.write(editedLine)
    else:
        w.write(first_line.rstrip("\n") +" " + value_for_plot + " " + str(value)+"\n")
        for nodes in lines:
            r = random.randint(0, len(valency)-1)
            editedLine = f"{nodes.rstrip()} {str(valency[r])}\n"
            w.write(editedLine)

    w.close()
    r.close()
    os.remove(filename)

def create_graph_file( nodes, edges, filename , seed):
    command_nodes_and_edges = "python gen_graph.py -grnm -n " + str(nodes) + " -m " + str(edges) + " --seed "+ str(seed) + " --out " + filename
    os.system(command_nodes_and_edges)

def genrate_f_dist_sample(no_samples):
    rv2 = f(dfn=10, dfd=50, loc=0, scale=1)
    x = np.linspace(rv2.ppf(0.0001), rv2.ppf(0.9999), no_samples)
    y = rv2.pdf(x)
    y = y/max(y)
    y = np.random.permutation(y)
    return y

def create_f_graph(path, folderIndex, index, nodes, edges, seed, value_for_plot, value, valency = None, ):
    subFolder = path/str(folderIndex)
    if(valency is None):
        subFolder = path/str(folderIndex)
    else:
        subFolder = path/(str(folderIndex) + "_valency")
    if not os.path.exists(subFolder):
        os.mkdir(subFolder)
    filename = str(index)
    create_graph_file(nodes, edges, filename, seed=seed+index)
    
    r = open(filename, "r")
    first_line = r.readline()
    
    filename_processed = subFolder/f"{str(index)}.txt"

    w = open(filename_processed, "w+")
    
    lines = r.readlines()

    if(valency is None):
        w.write(first_line.rstrip("\n") +" "+value_for_plot + " " + str(value)+"\n")
        for nodes in lines:
            editedLine = f"{nodes.rstrip()} {str(np.random.f(0.1, 90, 1)[0])}\n"
            w.write(editedLine)

    w.close()
    r.close()
    os.remove(filename)
    
if __name__ == "__main__":
    main()