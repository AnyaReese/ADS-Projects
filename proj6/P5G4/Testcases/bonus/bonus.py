import numpy as np
import subprocess
import time
import os
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

def generate_test_case_domino(n, domino_shapes_range=(1, 6), width_range=(10, 20)):
    domino_shapes = np.random.randint(*domino_shapes_range, n)
    strip_width = np.random.randint(*width_range)
    return domino_shapes, strip_width

def run_test(n, test_case_generator, exe_path, domino_shapes, strip_width):
    test_input = f"{n}\n{strip_width}\n"
    test_input += "\n".join(str(shape) for shape in domino_shapes)
    
    start_time = time.time()
    result = subprocess.run(exe_path, input=test_input, text=True, capture_output=True)
    elapsed = time.time() - start_time

    min_height = 4 * len(domino_shapes) / strip_width

    return elapsed, test_input, result.stdout, min_height

def run_tests(test_cases, test_case_generator, exe_paths, names):
    for name, exe_path in zip(names, exe_paths):
        result_dir = f'C:/Users/pc/Desktop/test/random/{name}/domino'
        times_file_path = os.path.join(result_dir, 'execution_times.txt')
        ratios_file_path = os.path.join(result_dir, 'approx_ratios.txt')

        if not os.path.exists(result_dir):
            os.makedirs(result_dir)

        times_file = open(times_file_path, 'w')
        ratios_file = open(ratios_file_path, 'w')

        elapsed_times = []
        approx_ratios = []

        for i, (domino_shapes, strip_width) in enumerate(test_cases):
            elapsed, test_input, result, min_height = run_test(len(domino_shapes), test_case_generator, exe_path, domino_shapes, strip_width)

            times_file.write(f'Number of rectangles: {len(domino_shapes)}, Time elapsed: {elapsed} seconds\n')

            dir_path = os.path.join(result_dir, f'test_case_{len(domino_shapes)}')
            if not os.path.exists(dir_path):
                os.mkdir(dir_path)
            with open(os.path.join(dir_path, 'input.txt'), 'w') as f:
                f.write(test_input)
            with open(os.path.join(dir_path, 'output.txt'), 'w') as f:
                f.write(result)

            max_height = float(result.strip())
            
            approx_ratio = max_height / min_height
            ratios_file.write(f'Number of rectangles: {len(domino_shapes)}, Approximation ratio: {approx_ratio}\n')

            elapsed_times.append(elapsed)
            approx_ratios.append(approx_ratio)

        times_file.close()
        ratios_file.close()

        plt.figure(dpi=600)
        plt.plot([len(domino_shapes) for domino_shapes, _ in test_cases], elapsed_times)
        plt.xlabel('Number of rectangles')
        plt.ylabel('Total execution time (seconds)')
        plt.title(f'[{name}] Execution time as a function of the number of rectangles')
        plt.savefig(os.path.join(result_dir, 'execution_times.png'))

        plt.figure(dpi=600)
        plt.plot([len(domino_shapes) for domino_shapes, _ in test_cases], approx_ratios)
        plt.xlabel('Number of rectangles')
        plt.ylabel('Approximation ratio')
        plt.title(f'[{name}] Approximation ratio as a function of the number of rectangles')
        plt.savefig(os.path.join(result_dir, 'approx_ratios.png'))

test_case_generators = [generate_test_case_domino]
test_case_names = ["domino"]

exe_paths = ["C:/Users/pc/Desktop/test/Domino.exe"]
names = ["Domino"]

test_case_index = 0
test_case_generator = test_case_generators[test_case_index]
test_case_name = test_case_names[test_case_index]

test_cases = [test_case_generator(n) for n in np.arange(10, 10000, 1000)]

run_tests(test_cases, test_case_generator, exe_paths, names)
