import numpy as np
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt 

plt.rcParams['font.family'] = 'Times New Roman'
plt.rcParams['font.sans-serif'] = ['NSimSun']
plt.rcParams['font.size'] = 18

# Define the data sizes
data_sizes = np.array([1, 1.5, 2, 2.5, 3, 4, 5, 10])

# Generate random running times for each dataset
times_SAS_ad = np.array([ ])  # Sample times for dataset 1
times_SAS = np.array([    ])  # Sample times for dataset 2
times_FFDH = np.array([   ])  # Sample times for dataset 3
times_NFDH = np.array([   ])  # Sample times for dataset 4
times_Sleator = np.array([])  # Sample times for dataset 5
times_SP = np.array([     ])  # Sample times for dataset 6

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
