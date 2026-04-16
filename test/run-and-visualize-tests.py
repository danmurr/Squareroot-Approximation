import pandas as pd
import matplotlib.pyplot as plt
import os
from scipy import stats
import numpy as np

partial_path = "../test/past-tests/"
folder = "2026-04-16_14_04_49.953382/"
file = "subsystem-sequence-1.csv"

print(os.listdir(partial_path))
max = -1
max_i = -1
for i, folder in enumerate(os.listdir(partial_path)):
    if os.path.getctime(partial_path + folder) > max:
        max = os.path.getctime(partial_path + folder)
        max_i = i

print("Most recent folder: " + os.listdir(partial_path)[max_i])
folder = os.listdir(partial_path)[max_i]

completed_tests = os.listdir(partial_path)
os.system("../build/runtests")
new_tests = os.listdir(partial_path)

for test in new_tests:
    if not(test in completed_tests):
        folder = test

csv_files = os.listdir(partial_path + folder)
print(csv_files)

statistics = ""

for file in csv_files:
    path = os.path.join(partial_path, folder, file)
    print(f"Processing: {path}")

    df = pd.read_csv(path)
    
    # Extract x (assumed to be the first column)
    x = df.iloc[:, 0]
    # Use log10 of x for regression to match the log-scale plot
    x_log = np.log10(x)
    
    num_y_vars = len(df.columns)

    # Prepare the plot
    plt.figure(figsize=(10, 6))

    for i in range(1, num_y_vars):
        column_name = df.columns[i]
        y = df.iloc[:, i]

        # 1. Calculate Linear Regression (y vs log10(x))
        slope, intercept, r_value, p_value, std_err = stats.linregress(x_log, y)
        r_squared = r_value**2
        
        statistics += f"{file} - {column_name}:\n"
        statistics += f"    Slope: {slope:.4f}\n"
        statistics += f"    Intercept: {intercept:.4f}\n"
        statistics += f"    R-squared: {r_squared:.4f}\n"

        print(f"  [Stats] {column_name}:")
        print(f"    Slope: {slope:.4f}")
        print(f"    Intercept: {intercept:.4f}")
        print(f"    R-squared: {r_squared:.4f}")

        # 2. Plot the actual data
        plt.scatter(x, y, label=f"{column_name} (R²={r_squared:.3f})")

        # 3. Optional: Plot the regression line
        # regression_line = slope * x_log + intercept
        # plt.plot(x, regression_line, linestyle='--', alpha=0.7)

    # Log scale on x-axis
    plt.xscale("log")

    # Labels and legend
    plt.xlabel(df.columns[0])
    plt.ylabel("Computation Time in Nanoseconds")
    plt.title(f"Performance Analysis: {file}")
    plt.legend()

    # Update filename for saving
    output_file = file.replace('.csv', '.png')
    save_path = os.path.join(partial_path, folder, output_file)
    
    # Save and clear
    plt.tight_layout()
    plt.savefig(save_path)
    plt.clf() 
    plt.close() # Better for memory management than clf() alone in loops
    
with open((partial_path + folder + "/statistics.txt"), "w") as f:
    f.write(statistics)