import numpy as np
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt 

plt.rcParams['font.family'] = 'Times New Roman'
plt.rcParams['font.sans-serif'] = ['NSimSun']
plt.rcParams['font.size'] = 18

# Define the data sizes
data_sizes = np.array([0, 200, 400, 600, 800, 2000, 4000, 5000, 6500, 8000])

# Generate random running times for each dataset
times_SAS_ad = np.array([ 0, 193, 402, 603, 920, 2085, 4335, 5830, 9665, 17165])  # Sample times for dataset 1
times_SAS = np.array([    0,  44, 132, 267, 360, 1860, 3278, 5349, 9358, 16497])  # Sample times for dataset 2
times_FFDH = np.array([   0,  16,  32,  53,  92,  299,  689,  997, 1489, 2096])  # Sample times for dataset 3
times_NFDH = np.array([   0,   4,   5,   8,  10,   20,   42,   39,   53, 58])  # Sample times for dataset 4
times_Sleator = np.array([0,   6,  10,  17,  21,   30,   31,   55,   95, 128])  # Sample times for dataset 5
times_SP = np.array([     0,  24,  44,  69, 108,  188,  380,  469,  605, 735])  # Sample times for dataset 6

datasets = [times_SAS_ad, times_SAS, times_FFDH, times_NFDH, times_Sleator, times_SP]
dataset_names = ["SAS_ad", "SAS", "FFDH", "NFDH", "Sleator", "SP"]

# Create plots for each dataset
for times, name in zip(datasets, dataset_names):
    # Create a smooth curve for the data
    xnew = np.linspace(data_sizes.min(), data_sizes.max(), 300)
    spl = make_interp_spline(data_sizes, times, k=1)
    y_smooth = spl(xnew)

    # Plot the data
    plt.plot(xnew, y_smooth, label=name)

plt.title("Running Time by Rectangle Number")
plt.xlabel("Data Size")
plt.ylabel("Running Time (µs)")
plt.legend()
plt.show()
