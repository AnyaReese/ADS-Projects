import random

def common_case():
    max_rectangles = 20
    max_height = 100
    # 确保给定的容器宽度足够大，可以容纳至少一个矩形
    given_width = random.randint(50, 100)

    n = random.randint(10, max_rectangles)

    with open('common_rec_20.txt', 'w') as file:
        file.write(f"{n} {given_width}\n")
        for _ in range(n):
            # 保证每个矩形的宽度不超过给定的宽度
            width = random.randint(1, given_width)
            height = random.randint(1, max_height)
            file.write(f"{width} {height}\n")

    print("Generate common_case done!")


def square_case():
    max_rectangles = 20
    max_height = 100
    # 确保给定的容器宽度足够大，可以容纳至少一个矩形
    given_width = random.randint(50, 100)

    n = random.randint(10, max_rectangles)

    with open('square_rec_20.txt', 'w') as file:
        file.write(f"{n} {given_width}\n")
        for _ in range(n):
            # 保证每个矩形的宽度不超过给定的宽度
            width = random.randint(1, given_width)
            height = width
            file.write(f"{width} {height}\n")

    print("Generate square_case done!")


def large_case():
    max_rectangles = 10000
    max_height = 100
    # 确保给定的容器宽度足够大，可以容纳至少一个矩形
    given_width = random.randint(50, 100)

    n = random.randint(100, max_rectangles)

    with open('large_rec_100.txt', 'w') as file:
        file.write(f"{n} {given_width}\n")
        for _ in range(n):
            # 保证每个矩形的宽度不超过给定的宽度
            width = random.randint(1, given_width)
            height = random.randint(1, max_height)
            file.write(f"{width} {height}\n")

    print("Generate large_case done!")

def all_narrow_case():
    max_rectangles = 20
    max_height = 100
    # 确保给定的容器宽度足够大，可以容纳至少一个矩形
    given_width = random.randint(50, 100)

    n = random.randint(10, max_rectangles)

    with open('all_narrow_rec_20.txt', 'w') as file:
        file.write(f"{n} {given_width}\n")
        for _ in range(n):
            # 保证每个矩形的宽度不超过给定的宽度
            width = random.randint(1, 5)
            height = random.randint(width, max_height)
            file.write(f"{width} {height}\n")
    
    print("Generate all_narrow_case done!")


def all_wide_case():
    max_rectangles = 20
    max_height = 100
    # 确保给定的容器宽度足够大，可以容纳至少一个矩形
    given_width = random.randint(50, 100)

    n = random.randint(10, max_rectangles)

    with open('all_wide_rec_20.txt', 'w') as file:
        file.write(f"{n} {given_width}\n")
        for _ in range(n):
            # 保证每个矩形的宽度不超过给定的宽度
            height = random.randint(1, 10)
            width = random.randint(height, given_width)
            file.write(f"{width} {height}\n")

    print("Generate all_wide_case done!")


if __name__ == "__main__":
    common_case()
    square_case()
    large_case()
    all_narrow_case()
    all_wide_case()
