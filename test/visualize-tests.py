import pandas as pd
import matplotlib.pyplot as plt

# Load CSV
partial_path = "./past-tests/"
folder = "2026-01-30_15_18_46.162053168/"
file = "sequence-1.csv"
df = pd.read_csv(partial_path + folder + file)

# Extract columns
x = df.iloc[:, 0]
y1 = df.iloc[:, 1]
y2 = df.iloc[:, 2]

# Create scatter plot
plt.scatter(x, y1, label=df.columns[1])
plt.scatter(x, y2, label=df.columns[2])

# Log scale on x-axis
plt.xscale("log")

# Labels and legend
plt.xlabel(df.columns[0])
plt.ylabel("Computation Time in Nanoseconds")
plt.legend()

file = file.replace('.csv', '.png')
# Show plot
plt.tight_layout()
plt.savefig(partial_path + folder + file)
