import random
# file_ending = "698"
# r = open(f"Raw_graphs/{file_ending}.edges", "r")
# w = open(f"processed_graphs/facebook_{file_ending}.edges", "w")
# prop = open(f"processed_graphs/facebook_{file_ending}.prop", "w")

file_ending = "syn_graph_seed0_n40_m80"
r = open(f"Raw_graphs/{file_ending}.inf", "r")
w = open(f"processed_graphs/{file_ending}.edges", "w")
prop = open(f"processed_graphs/{file_ending}.prop", "w")


lines = r.readlines()
num_nodes = 0
num_edges = 0
lowest = 99999999

edgesSet = set()

for line in lines:
    numbers = [int(n) for n in line.split()]
    lowest = min(lowest, min(numbers))

print(f"The lowest node is:{lowest}")
random.seed(1) 
for line in lines:
    numbers = [int(n) for n in line.split()]
    numbers = sorted([x-lowest for x in numbers])
    num_nodes = max(num_nodes, max(numbers))
    edgesSet.add(tuple(numbers))


for node1, node2 in edgesSet:
    editedLine = f"{node1} {node2} {str(random.uniform(0,0.5))}\n"
    w.write(editedLine)
    num_edges += 1

prop.write(f"nodes: {num_nodes}, edges: {num_edges}")
