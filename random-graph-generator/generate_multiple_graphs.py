
import os
import random
import shutil
from pathlib import Path


def main():
    valency_probs = [0.4, 0.6, 0.8]
    # generate_edge_degree_graphs()
    # generate_edge_degree_graphs(valency= valency_probs)
    # generate_numNodes_graphs()
    # generate_numNodes_graphs(valency = valency_probs)
    # generate_data_for_vary_k()
    generate_data_for_vary_k(valency = valency_probs)

    
def get_relative_path(value_for_plot):
    # relative_path = Path("C:/Users/mabet/OneDrive - Aarhus Universitet/Datalogi/Bachelor projekt/BSc_project/GraphsGeneration/processed_graphs")
    relative_path = Path("C:/Users/chris/Documents/6. Semester/Bachelor Project/BSc_project/GraphsGeneration/processed_graphs")

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

def generate_numNodes_graphs(Valency = None):
    num_nodes = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
    edge_degree = 2
    num_samples = 5
    value_for_plot = "num_nodes_edgedegree2"
    path = get_relative_path(value_for_plot)
    for folder_index, nodes in enumerate(num_nodes):
        for sample_index in range(num_samples):
            edges = edge_degree*nodes
            create_processed_graph_files(path, folder_index, sample_index, nodes, edges, 1, value_for_plot, nodes, Valency)

    
def generate_edge_degree_graphs(valency = None):
    value_for_plot = "edge_degree"
    relative_path_processed = get_relative_path(value_for_plot)
    nodes = 80
    edges = [80,120,160,200,240,280,320]
    for index,i in enumerate(edges):
        value = i/nodes
        create_processed_graph_files(relative_path_processed, index, nodes, i, 1, value_for_plot, value, valency)

def generate_data_for_vary_k(valency = None):
    value_for_plot = "Varying K"
    relative_path_processed = get_relative_path(value_for_plot)
    nodes = 60
    edges = 2*nodes
    numSamples = 5
    for file_index in range(numSamples):
        create_processed_graph_files(relative_path_processed, 1, file_index, nodes, edges, 1, value_for_plot, "k", valency)
        
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
    # r.close()
    os.remove(filename)

def create_graph_file( nodes, edges, filename , seed):
    command_nodes_and_edges = "python gen_graph.py -grnm -n " + str(nodes) + " -m " + str(edges) + " --seed "+ str(seed) + " --out " + filename
    os.system(command_nodes_and_edges)

if __name__ == "__main__":
    main()