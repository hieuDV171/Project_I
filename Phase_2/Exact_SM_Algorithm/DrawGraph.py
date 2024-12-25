import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("Phase_2\\Exact_SM_Algorithm\\results.csv")

plt.figure(figsize=(10, 6))
for algo in data['Algorithm'].unique():
    subset = data[data['Algorithm'] == algo]
    plt.plot(subset['Testcase'], subset['Time(ns)'], marker='o', label=algo)

plt.xticks(data['Testcase'], data['Testcase'].astype(int))
plt.xlabel("Test cases")
plt.ylabel("Execution Time (ns)")
plt.title("String Matching Algorithm Performance")
plt.legend()
plt.grid(True)
plt.show()