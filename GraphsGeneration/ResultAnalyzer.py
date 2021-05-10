from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt

path = Path("../GraphsGeneration/processed_graphs/num_nodes_edgedegree2/output/results.txt")

with open(path, "r") as r:
    graphTitle = r.readline()
    xAxis = r.readline().strip()
    datapoints = r.readlines()

refined = [x.strip() for x in datapoints]
refined = [x.split() for x in refined]
refined = [[float(j) for j in x] for x in refined]

df = pd.DataFrame.from_records(refined)
grouped = df.groupby(0)
print(grouped)
df.groupby(0).boxplot(subplots=False, sharex=True, use_index=True)
ax = df.groupby(0).mean().plot()
ax.set_xlabel(xAxis)
ax.set_ylabel("seconds")
ax.set_title("Mean time")
medianPlot = df.groupby(0).median().plot()
medianPlot.set_xlabel(xAxis)
medianPlot.set_ylabel("seconds")
medianPlot.set_title("Median time")
print(df.groupby(0).describe())
plt.show()




    
    
