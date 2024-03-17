import itertools
import numpy as np


def calculate_the_min_var(data):
    # 生成所有可能的数字组合
    all_combinations = itertools.product(*(data[key] for key in sorted(data.keys())))

    min_variance = float('inf')
    min_combination = None

    # 遍历所有组合以找到方差最小的组合
    for combination in all_combinations:
        variance = np.var(combination)
        if variance < min_variance:
            min_variance = variance
            min_combination = combination

    return min_variance
