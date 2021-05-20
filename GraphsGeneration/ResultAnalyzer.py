from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt
import math
from collections import Counter
import seaborn as sns
import numpy as np
import os

#takes tuples list and gives two lists
def _zip(zs):
    xs, ys = [], []
    for x, y in zs:
        xs.append(x)
        ys.append(y)
    return xs, ys

def structure_data_for_plot(datapoints):
    refined = [x.strip() for x in datapoints]
    refined = [x.split() for x in refined]
    refined = [[float(j) for j in x] for x in refined]
    return refined

def visualize (dir, save_path, experiment_string, log_scale = True):
    graph_title =  str(dir).split("Results/",1)[1]
    if(experiment_string == "valency"):
        graph_title = graph_title + " " +experiment_string
    refined_list =[]
    title_list = []
    xAxis_list = []
    for path in os.listdir(dir):
        path_in_str = str(path)
        if experiment_string =="valency" and "valency" in path_in_str:        
            with open(str(dir)+ "/"+path_in_str, "r") as r:
                title = path_in_str.split("_",1)[1].replace('.txt','')
                _ = r.readline()
                title_list.append(title)
                xAxis = r.readline().strip()
                xAxis_list.append(xAxis)
                datapoints = r.readlines()
                refined = structure_data_for_plot(datapoints)
                refined_list.append(refined)
        elif experiment_string != "valency" and "valency" not in path_in_str:        
            with open(str(dir)+ "/"+path_in_str, "r") as r:
                title = path_in_str.split("_",1)[1].replace('.txt','')
                graphTitle = r.readline()
                title_list.append(title)
                xAxis = r.readline().strip()
                xAxis_list.append(xAxis)
                datapoints = r.readlines()
                refined = structure_data_for_plot(datapoints)
                refined_list.append(refined)
    
    plt.figure(figsize = (10, 8))
    counter=0
    for l in refined_list:
        df = pd.DataFrame.from_records(l)
        # sns.lineplot(data = df, y = df[1], x = df[0], marker="o",label=title_list[counter], dashes=False, ci=100) # ci: Size of the confidence interval to draw when aggregating with an estimator
        sns.lineplot(data = df, y = 1-df[1]/10, x = df[0], marker="o",label=title_list[counter], dashes=False, ci=0) # ci: Size of the confidence interval to draw when aggregating with an estimator
        counter +=1
    # sns.lineplot(data = df, y = df[1], x = df[0], marker="o", err_style="bars", ci=100) # ci: Size of the confidence interval to draw when aggregating with an estimator
    # sns.lineplot(data = df, y = df[1]+5, x = df[0], marker="o", dashes=False,  ci=100) # ci: Size of the confidence interval to draw when aggregating with an estimator
    if log_scale:
        graph_title_inPlot = graph_title + " - log scaled y"
    else: 
        graph_title_inPlot = graph_title +  "@K - Precision of 1step compared to 2step approach " 
    plt.title(graph_title_inPlot, size = 16)
    # plt.legend(title="legends", loc='upper left')
    plt.xlabel(xAxis_list[0])
    plt.ylabel("errors")
    # plt.ylabel("seconds")
    if( log_scale):
        plt.yscale('log')
    # plt.ylim(0, 10**4)
    plt.legend(loc="upper left")
    plt.savefig(str(save_path) + '/' + graph_title +'.png', bbox_inches='tight')
    plt.show()
       
# path = Path("/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/GraphsGeneration/processed_graphs/Results/NumNodes/results_1step_valency.txt")
# dir = Path("/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/GraphsGeneration/processed_graphs/Results/NumNodes")
# dir = Path("/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/GraphsGeneration/processed_graphs/Results/Varying_k")
# dir = Path("/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/GraphsGeneration/processed_graphs/Results/EdgeDegree")
dir = "/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/GraphsGeneration/processed_graphs/Results/Precision"
save_path = Path("/Users/sebastianloeschcke/Desktop/6.semester/BSc/BSc_project/GraphsGeneration/processed_graphs/Results/Saved_plots")
visualize(dir, save_path, "", log_scale = False)
