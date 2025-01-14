import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("Phase_2\\Exact_SM_Algorithm\\SinglePatternMatching\\results.csv")

plt.figure(figsize=(10, 6))
for algo in data['Algorithm'].unique():
    subset = data[data['Algorithm'] == algo]
    plt.plot(subset['Testcase'], subset['Time(us)'], marker='o', label=algo)

plt.xticks(data['Testcase'], data['Testcase'].astype(int))
plt.xlabel("Test cases")
plt.ylabel("Execution Time (us)")
plt.title("String Matching Algorithm Performance")
plt.legend()
plt.grid(True)

ax = plt.gca()
def format_coord(x, y):
    return f"x = {x:.2f}, y = {y:.2f}"
ax.format_coord = format_coord

plt.show()