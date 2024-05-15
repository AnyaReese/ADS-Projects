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
from scipy.interpolate import make_interp_spline

class Rect:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height

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

def generate_test_case_width_ratio(n, width_range=(1, 10), height_range=(1, 10)):
    widths = np.random.randint(*width_range, n)
    heights = np.random.randint(*height_range, n)
    max_width = max(widths)
    strip_widths = np.ceil(np.linspace(2.5*max_width, 2.5*max_width, 1)).astype(int)

    for i in range(1, len(strip_widths)):
        if strip_widths[i] == strip_widths[i-1]:
            strip_widths[i] += 1

    strip_widths = np.sort(strip_widths)

    return [(widths, heights, strip_width) for strip_width in strip_widths]


test_case_generators = [generate_test_case_random, generate_test_case_random_square, generate_test_case_greater_than_sum, generate_test_case_greater_than_sum_square, generate_test_case_width_ratio]
test_case_names = ["random", "random_square", "greater_than_sum", "greater_than_sum_square", "width_ratio"]

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
    return rects, max(rect.y + rect.height for rect in rects)

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

def run_tests(test_cases, test_case_generator, exe_paths, names):
    for name, exe_path in zip(names, exe_paths):
        if not os.path.exists(f'C:/Users/pc/Desktop/test/random/{name}'):
            os.makedirs(f'C:/Users/pc/Desktop/test/random/{name}')
        if os.path.exists(f'C:/Users/pc/Desktop/test/random/{name}/test_cases_{test_case_name}'):
            shutil.rmtree(f'C:/Users/pc/Desktop/test/random/{name}/test_cases_{test_case_name}')
        os.makedirs(f'C:/Users/pc/Desktop/test/random/{name}/test_cases_{test_case_name}')

        result_dir = f'C:/Users/pc/Desktop/test/random/{name}/test_cases_{test_case_name}'
        times_file_path = os.path.join(result_dir, 'execution_times.txt')
        ratios_file_path = os.path.join(result_dir, 'approx_ratios.txt')
        
        if not os.path.exists(result_dir):
            os.makedirs(result_dir)

        times_file = open(times_file_path, 'w')
        ratios_file = open(ratios_file_path, 'w')

        elapsed_times = []
        approx_ratios = []

        for i, (widths, heights, strip_width) in enumerate(test_cases):
            elapsed, test_input, result, min_height = run_test(len(widths), test_case_generator, exe_path, widths, heights, strip_width)
            print(f"[{name}] Number of rectangles: {len(widths)}, Time elapsed: {elapsed} seconds")
            times_file.write(f'Number of rectangles: {len(widths)}, Time elapsed: {elapsed} seconds\n')

            dir_path = os.path.join(f'C:/Users/pc/Desktop/test/random/{name}/test_cases_{test_case_name}', f'test_case_{len(widths)}')
            if not os.path.exists(dir_path):
                os.mkdir(dir_path)
            with open(os.path.join(dir_path, 'input.txt'), 'w') as f:
                f.write(test_input)
            with open(os.path.join(dir_path, 'output.txt'), 'w') as f:
                f.write(result)

            test_input_lines = test_input.strip().split("\n")
            widths = [int(line.split()[0]) for line in test_input_lines[1:-1]]
            heights = [int(line.split()[1]) for line in test_input_lines[1:-1]]
            rects, max_height = parse_output(result, widths, heights)
            visualize(rects, dir_path)
            
            approx_ratio = max_height / min_height
            print(f"[{name}] Approximation ratio: {approx_ratio}")
            ratios_file.write(f'Number of rectangles: {len(widths)}, Approximation ratio: {approx_ratio}\n')

            elapsed_times.append(elapsed)
            approx_ratios.append(approx_ratio)

        times_file.close()
        ratios_file.close()
            
        all_elapsed_times.append(elapsed_times)
        all_approx_ratios.append(approx_ratios)

        strip_widths = [strip_width for _, _, strip_width in test_cases]
        xnew = np.linspace(min(strip_widths), max(strip_widths), 500)

        strip_widths, elapsed_times = zip(*sorted(zip(strip_widths, elapsed_times)))
        strip_widths, approx_ratios = zip(*sorted(zip(strip_widths, approx_ratios)))

        plt.figure(dpi=600)
        for name, elapsed_times in zip(names, all_elapsed_times):
            spl = make_interp_spline(strip_widths, elapsed_times, k=1) 
            elapsed_times_smooth = spl(xnew)
            plt.plot(max(widths)/xnew, elapsed_times_smooth, label=name)

        plt.xlabel('Strip width')
        plt.ylabel('Total execution time (seconds)')
        plt.title('Execution time as a function of strip width (all algorithms)')
        plt.legend()
        plt.grid(True)
        plt.savefig('C:/Users/pc/Desktop/test/random/execution_time_vs_strip_width.png')
        plt.close()

        plt.figure(dpi=600)
        for name, approx_ratios in zip(names, all_approx_ratios):
            spl = make_interp_spline(strip_widths, approx_ratios, k=1)
            approx_ratios_smooth = spl(xnew)
            plt.plot(max(widths)/xnew, approx_ratios_smooth, label=name)

        plt.xlabel('Strip width')
        plt.ylabel('Approximation ratio')
        plt.title('Approximation ratio as a function of strip width (all algorithms)')
        plt.legend()
        plt.grid(True)
        plt.savefig('C:/Users/pc/Desktop/test/random/approx_ratio_vs_strip_width.png')
        plt.close()

    strip_widths = [strip_width for _, _, strip_width in test_cases]
    xnew = np.linspace(min(strip_widths), max(strip_widths), 500)

    for name, elapsed_times in zip(names, all_elapsed_times):
        spl = make_interp_spline(strip_widths, elapsed_times, k=1)
        elapsed_times_smooth = spl(xnew)
        plt.plot(max(widths)/xnew, elapsed_times_smooth, label=name)

    plt.xlabel('Strip width')
    plt.ylabel('Total execution time (seconds)')
    plt.title('Execution time as a function of strip width (all algorithms)')
    plt.legend()
    plt.grid(True)
    plt.savefig('C:/Users/pc/Desktop/test/random/execution_time_vs_strip_width.png')
    plt.close()

    for name, approx_ratios in zip(names, all_approx_ratios):
        spl = make_interp_spline(strip_widths, approx_ratios, k=1)
        approx_ratios_smooth = spl(xnew)
        plt.plot(max(widths)/xnew, approx_ratios_smooth, label=name)

    plt.xlabel('Strip width')
    plt.ylabel('Approximation ratio')
    plt.title('Approximation ratio as a function of strip width (all algorithms)')
    plt.legend()
    plt.grid(True)
    plt.savefig('C:/Users/pc/Desktop/test/random/approx_ratio_vs_strip_width.png')
    plt.close()

exe_paths = ["C:/Users/pc/Desktop/test/BL.exe", "C:/Users/pc/Desktop/test/FFDH.exe", "C:/Users/pc/Desktop/test/NFDH.exe", "C:/Users/pc/Desktop/test/Sleator.exe", "C:/Users/pc/Desktop/test/SP.exe"]
names = ["BL", "FFDH", "NFDH", "Sleator", "SP"]

test_case_index = 4
test_case_generator = test_case_generators[test_case_index]
test_case_name = test_case_names[test_case_index]

test_cases = test_case_generator(30)

run_tests(test_cases, test_case_generator, exe_paths, names)