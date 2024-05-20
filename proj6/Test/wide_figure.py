import numpy as np
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt 

plt.rcParams['font.family'] = 'Times New Roman'
plt.rcParams['font.sans-serif'] = ['NSimSun']
plt.rcParams['font.size'] = 18

# Define the data sizes
data_sizes = np.array([ 1000, 2000, 4000,  8000])

# Generate random running times for each dataset
times_SAS_ad = np.array([1.202858,1.195894 ,1.195942 ,1.186722])  # Sample times for dataset 1
times_SAS = np.array([2.911135,  3.314078,3.053483,2.964658])  # Sample times for dataset 2
times_FFDH = np.array([  1.052668,  1.026570,1.031013,1.035820])  # Sample times for dataset 3
times_NFDH = np.array([  1.189174,  1.179194,  1.172530, 1.171174])  # Sample times for dataset 4
times_Sleator = np.array([1.292416, 1.281214,1.305048,1.308127])  # Sample times for dataset 5
times_SP = np.array([1.289691,1.312049, 1.316831, 1.315857])  # Sample times for dataset 6

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

plt.title("Approximation ratio of All Wide Rectangles")
plt.xlabel("Data Size")
plt.ylabel("Approximation ratio")
# plt.legend()
plt.show()