import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("Phase_2\\Exact_SM_Algorithm\\MultiPatternMatching\\MP_results.csv")

plt.figure(figsize=(10, 6))

for algo in data['Algorithm'].unique():
    subset = data[data['Algorithm'] == algo]
    plt.plot(subset['Testcase'], subset['Time(us)'], marker='o', label=algo)

plt.xticks(data['Testcase'].unique(), data['Testcase'].unique().astype(int))
plt.xlabel("Test Cases")
plt.ylabel("Execution Time (us)")
plt.title("Multi-Pattern Matching Algorithm Performance")
plt.legend(title="Algorithm")
plt.grid(True)

ax = plt.gca()
def format_coord(x, y):
    return f"x = {x:.2f}, y = {y:.2f}"
ax.format_coord = format_coord

plt.show()