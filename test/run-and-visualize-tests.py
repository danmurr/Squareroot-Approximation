import pandas as pd
import matplotlib.pyplot as plt
import os

partial_path = "./past-tests/"
folder = "2026-01-30_19_36_13.745626386/"
file = "subsystem-sequence-1.csv"

completed_tests = os.listdir(partial_path)
os.system("../build/runtests")
new_tests = os.listdir(partial_path)

for test in new_tests:
    if not(test in completed_tests):
        folder = test

csv_files = os.listdir(partial_path + folder)
print(csv_files)

for file in csv_files:
    path = partial_path + folder + "/" + file
    print(path)

    df = pd.read_csv(path)

    # Extract columns
    num_y_vars = len(df.columns)

    x = df.iloc[:, 0]

    y_vars = []
    for i in range(1,num_y_vars):
        y_vars.append(df.iloc[:,i])

    for i in range(1, num_y_vars):
        plt.scatter(x, y_vars[i-1], label=df.columns[i])

    # Log scale on x-axis
    plt.xscale("log")

    # Labels and legend
    plt.xlabel(df.columns[0])
    plt.ylabel("Computation Time in Nanoseconds")
    plt.legend()

    file = file.replace('.csv', '.png')
    # Show plot
    plt.tight_layout()
    plt.savefig(partial_path + folder + "/" + file)
    plt.clf() # Clear the entire current figure
