
import os
import random


def main():
    relative_path = "./generated_graphs/"
    value_for_plot= "edge_degree"
    # vertices = [40,80,160,240,320]
    edges = [80,120,160,200,240,280,320]
    for i in edges:
        filename = relative_path+value_for_plot+str(i)
        create_graph_file( 80, i, filename, seed=i)
        # create_graph_file( i, i*2, filename, seed=i) #vertices

        r = open(filename, "r")
        first_line = r.readline()

        relative_path_processed = "./generated_graphs/processed_"
        filename = relative_path_processed +value_for_plot+str(i)
        w = open(filename, "w")
        lines = r.readlines()[1:]

        
        lowest = 99999999

        edgesSet = set()

        for line in lines:
            numbers = [int(n) for n in line.split()]
            lowest = min(lowest, min(numbers))
        random.seed(i) 
        for line in lines:
            numbers = [int(n) for n in line.split()]
            numbers = sorted([x-lowest for x in numbers])
            edgesSet.add(tuple(numbers))

        w.write(first_line.rstrip("\n") +" "+value_for_plot +"\n" )
        a = open(filename, "a")
        for node1, node2 in edgesSet:
            editedLine = f"{node1} {node2} {str(random.uniform(0,1))}\n"
            a.write(editedLine)


def create_graph_file( nodes, edges, filename , seed):
    command_nodes_and_edges = "python3 gen_graph.py -grnm -n " + str(nodes) + " -m " + str(edges) + " --seed "+ str(seed) + " --out " + filename
    os.system(command_nodes_and_edges)



def prepend_line(file_name, line):
    """ Insert given string as a new line at the beginning of a file """
    # define name of temporary dummy file
    dummy_file = file_name + '.bak'
    # open original file in read mode and dummy file in write mode
    with open(file_name, 'r') as read_obj, open(dummy_file, 'w') as write_obj:
        # Write given line to the dummy file
        write_obj.write(line + '\n')
        # Read lines from original file one by one and append them to the dummy file
        for line in read_obj:
            write_obj.write(line)
    # remove original file
    os.remove(file_name)
    # Rename dummy file as the original file
    os.rename(dummy_file, file_name)



if __name__ == "__main__":
    main()