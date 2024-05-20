import numpy as np
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt 

plt.rcParams['font.family'] = 'Times New Roman'
plt.rcParams['font.sans-serif'] = ['NSimSun']
plt.rcParams['font.size'] = 18

# Define the data sizes
data_sizes = np.array([ 100, 500, 2000, 5000, 10000])

# Generate random running times for each dataset
times_SAS_ad = np.array([  1.254433, 1.275280,1.262933,1.267878,1.267134])  # Sample times for dataset 1
times_SAS = np.array([      1.603611, 1.534873,1.513650,1.501400,1.491718])  # Sample times for dataset 2
times_FFDH = np.array([     1.145568, 1.137057 ,1.124125,1.129400,1.128214])  # Sample times for dataset 3
times_NFDH = np.array([      1.254433, 1.275420,1.262933 ,1.267878,1.267136])  # Sample times for dataset 4
times_Sleator = np.array([   1.282155, 1.292547, 1.276720,1.280777,1.280124])  # Sample times for dataset 5
times_SP = np.array([  1.270038, 1.292069, 1.278534,1.282278,1.281533])  # Sample times for dataset 6

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

plt.title("Approximation ratio of All Square Rectangles")
plt.xlabel("Data Size")
plt.ylabel("Approximation ratio")
# plt.legend()
plt.show()
