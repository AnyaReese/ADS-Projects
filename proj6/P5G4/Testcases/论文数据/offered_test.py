import numpy as np
import subprocess
import time
import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import re
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import shutil
import glob
from scipy.interpolate import make_interp_spline, BSpline

class Rect:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height

optimal_heights = {
    'BENG': [30, 57, 84, 107, 134, 36, 67, 101, 126, 156],
    'CGCUT': [23, 63, 636],
    'NGCUT': [23, 30, 28, 20, 36, 31, 20, 33, 49, 80, 52, 87],
}

def generate_test_case_random(n, width_range=(1, 10), height_range=(1, 10)):
    widths = np.random.randint(*width_range, n)
    heights = np.random.randint(*height_range, n)
    strip_width = max(widths) + np.random.randint(0, np.sum(widths) - max(widths))
    return widths, heights, strip_width

def generate_test_case_random_square(n, side_range=(1, 10)):
    sides = np.random.randint(*side_range, n)
    strip_width = max(sides) + np.random.randint(0, np.sum(sides) - max(sides))
    return sides, sides, strip_width

def generate_test_case_greater_than_sum(n, width_range=(1, 10), height_range=(1, 10)):
    widths = np.random.randint(*width_range, n)
    heights = np.random.randint(*height_range, n)
    strip_width = np.sum(widths) + np.random.randint(0, max(widths))
    return widths, heights, strip_width

def generate_test_case_greater_than_sum_square(n, side_range=(1, 10)):
    sides = np.random.randint(*side_range, n)
    strip_width = np.sum(sides) + np.random.randint(0, max(sides))
    return sides, sides, strip_width

def generate_test_case_from_file(file_path):
    with open(file_path, 'r') as f:
        lines = f.read().splitlines()

    try:
        n = int(lines[0])
    except ValueError:
        print(f"Skipping file {file_path} due to incorrect format.")
        return None

    strip_width = int(lines[1])
    rects_info = lines[2:]

    widths = []
    heights = []

    for rect_info in rects_info:
        if not rect_info.strip():
            continue
        parts = rect_info.split()
        if len(parts) != 3:
            continue
        _, width, height = parts
        widths.append(int(width))
        heights.append(int(height))

    return widths, heights, strip_width

test_case_generators = [generate_test_case_random, generate_test_case_random_square, generate_test_case_greater_than_sum, generate_test_case_greater_than_sum_square]
test_case_names = ["random", "random_square", "greater_than_sum", "greater_than_sum_square"]

def run_test(n, test_case_generator, exe_path, widths, heights, strip_width):
    test_input = f"{n}\n"
    test_input += "\n".join(f"{w} {h}" for w, h in zip(widths, heights))
    test_input += f"\n{strip_width}"
    
    start_time = time.time()
    result = subprocess.run(exe_path, input=test_input, text=True, capture_output=True)
    elapsed = time.time() - start_time

    widths_array = np.array(widths)
    heights_array = np.array(heights)

    return elapsed, test_input, result.stdout, sum(widths_array * heights_array) / strip_width

def parse_output(output, widths, heights):
    output_lines = output.strip().split("\n")
    rects = []
    for i, line in enumerate(output_lines):
        split_line = line.split(":")
        if len(split_line) < 2:
            print(f"Error parsing output: {line}")
            continue
        _, coords = split_line
        x, y = map(int, re.findall(r'\d+', coords))
        rects.append(Rect(x, y, widths[i], heights[i]))
    return rects, max(rect.y + rect.height for rect in rects)\

def calculate_approximation_ratio(optimal_height, result_height):
    return result_height / optimal_height

def visualize(rects, dir_path):
    if not rects:
        print("No rectangles to visualize")
        return
    
    fig, ax = plt.subplots(dpi=600, figsize=(10, 10))

    cmap = plt.get_cmap("cool")
    
    for i, rect in enumerate(rects):
        color = cmap(i / len(rects))

        patch = patches.Rectangle((rect.x, rect.y), rect.width, rect.height, linewidth=1, edgecolor='black', facecolor=color)
        ax.add_patch(patch)

        ax.text(rect.x + rect.width / 2, rect.y + rect.height / 2, str(i), ha='center', va='center', fontsize=15, color='black')

    plt.xlim(0, max(rect.x + rect.width for rect in rects))
    plt.ylim(0, max(rect.y + rect.height for rect in rects))
    plt.xlabel("Width")
    plt.ylabel("Height")
    plt.title("Strip Packing Visualization")
    plt.savefig(os.path.join(dir_path, 'visualization.png'))
    plt.close()

all_elapsed_times = []
all_approx_ratios = []

def run_tests(test_cases, test_case_generator, test_case_name, exe_paths, names):
    global_stats_dir = os.path.join(f'C:/Users/pc/Desktop/test/literarily offered/{test_case_name}', 'global_stats')
    if not os.path.exists(global_stats_dir):
        os.makedirs(global_stats_dir)

    elapsed_times_dict = {name: {} for name in names}
    approx_ratios_dict = {name: {} for name in names}

    all_elapsed_times = []
    all_approx_ratios = []
    all_num_rectangles = []

    for exe_path, name in zip(exe_paths, names):
        elapsed_times = []
        approx_ratios = []
        num_rectangles = []

        for i, (test_case_file_name, test_case) in enumerate(test_cases):
            if test_case is None:
                continue
            widths, heights, strip_width = test_case
            elapsed, test_input, result, min_height = run_test(len(widths), test_case_generator, exe_path, widths, heights, strip_width)
            print(f"[{name}] Number of rectangles: {len(widths)}, Time elapsed: {elapsed} seconds")

            test_case_dir = os.path.join(f'C:/Users/pc/Desktop/test/literarily offered/{test_case_name}', test_case_file_name, name)
            if not os.path.exists(test_case_dir):
                os.makedirs(test_case_dir)

            dir_path = os.path.join(test_case_dir, f'test_case_{len(widths)}')
            os.makedirs(dir_path, exist_ok=True)
            with open(os.path.join(dir_path, 'input.txt'), 'w') as f:
                f.write(test_input)
            with open(os.path.join(dir_path, 'output.txt'), 'w') as f:
                f.write(result)

            test_input_lines = test_input.strip().split("\n")
            widths = [int(line.split()[0]) for line in test_input_lines[1:-1]]
            heights = [int(line.split()[1]) for line in test_input_lines[1:-1]]
            rects, max_height = parse_output(result, widths, heights)
            visualize(rects, dir_path)

            test_case_name_split = test_case_file_name.split('_')
            test_case_number_str = test_case_name_split[-1].lstrip(test_case_name_split[0])
            test_case_category = test_case_name_split[0]
            test_case_number = int(test_case_number_str) - 1
            optimal_height = optimal_heights[test_case_category][test_case_number]

            approx_ratio = calculate_approximation_ratio(optimal_height, max_height)

            print(f"[{name}] Approximation ratio: {approx_ratio}")

            elapsed_times.append(elapsed)
            approx_ratios.append(approx_ratio)
            num_rectangles.append(len(widths)) 

            num_rects = len(widths)
            if num_rects not in elapsed_times_dict[name]:
                elapsed_times_dict[name][num_rects] = []
            if num_rects not in approx_ratios_dict[name]:
                approx_ratios_dict[name][num_rects] = []
        
            elapsed_times_dict[name][num_rects].append(elapsed)
            approx_ratios_dict[name][num_rects].append(approx_ratio)

        all_elapsed_times.append(elapsed_times)
        all_approx_ratios.append(approx_ratios)
        all_num_rectangles.append(num_rectangles)

        with open(os.path.join(global_stats_dir, f'{name}_data.txt'), 'w') as f:
            for elapsed, approx_ratio, num_rectangle in zip(elapsed_times, approx_ratios, num_rectangles):
                f.write(f'Number of rectangles: {num_rectangle}, Time elapsed: {elapsed}, Approximation ratio: {approx_ratio}\n')

    plt.figure(dpi=600)
    for name in names:
        avg_elapsed_times = []
        for num_rects in sorted(elapsed_times_dict[name].keys()):
            avg_elapsed_times.append((num_rects, np.mean(elapsed_times_dict[name][num_rects])))

        avg_elapsed_times.sort(key=lambda x: x[0])
        num_rects, times = zip(*avg_elapsed_times)

        num_rects_new = np.linspace(min(num_rects), max(num_rects), 500)
        spl = make_interp_spline(num_rects, times, k=2)
        times_smooth = spl(num_rects_new)
        
        plt.plot(num_rects_new, times_smooth, label=name)

    plt.xlabel('Number of rectangles')
    plt.ylabel('Average execution time (seconds)')
    plt.title(f'Average execution time as a function of the number of rectangles')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(global_stats_dir, f'average_execution_times_smooth.png'))
    plt.close()

    plt.figure(dpi=600)
    for name in names:
        avg_approx_ratios = []
        for num_rects in sorted(approx_ratios_dict[name].keys()):
            avg_approx_ratios.append((num_rects, np.mean(approx_ratios_dict[name][num_rects])))

        avg_approx_ratios.sort(key=lambda x: x[0])
        num_rects, ratios = zip(*avg_approx_ratios)

        num_rects_new = np.linspace(min(num_rects), max(num_rects), 500)
        spl = make_interp_spline(num_rects, ratios, k=2)
        ratios_smooth = spl(num_rects_new)
        
        plt.plot(num_rects_new, ratios_smooth, label=name)

    plt.xlabel('Number of rectangles')
    plt.ylabel('Average approximation ratio')
    plt.title(f'Average approximation ratio as a function of the number of rectangles')
    plt.legend()
    plt.grid(True)
    plt.savefig(os.path.join(global_stats_dir, f'average_approx_ratios_smooth.png'))
    plt.close()

exe_paths = ["C:/Users/pc/Desktop/test/BL.exe", "C:/Users/pc/Desktop/test/FFDH.exe", "C:/Users/pc/Desktop/test/NFDH.exe", "C:/Users/pc/Desktop/test/Sleator.exe", "C:/Users/pc/Desktop/test/SP.exe"]
names = ["BL", "FFDH", "NFDH", "Sleator", "SP"]

test_case_directory_paths = ["C:/Users/pc/Desktop/test/literarily offered/datasets/BENG", 
                             "C:/Users/pc/Desktop/test/literarily offered/datasets/CGCUT", 
                             "C:/Users/pc/Desktop/test/literarily offered/datasets/NGCUT"]

for test_case_directory_path in test_case_directory_paths:
    all_elapsed_times = []
    all_approx_ratios = []
    test_cases = []
    for file_path in glob.glob(f"{test_case_directory_path}/*.TXT"):
        test_case_name = os.path.basename(test_case_directory_path) + '_' + os.path.basename(file_path).replace('.TXT', '')
        test_case = generate_test_case_from_file(file_path)
        if test_case is not None:
            test_cases.append((test_case_name, test_case))
    run_tests(test_cases, generate_test_case_from_file, test_case_name, exe_paths, names)