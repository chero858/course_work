import random


def degree(num):
    count = 0
    while num > 1:
        num >>= 1
        count += 1
    return count


if __name__ == '__main__':
    # nums = [0, 2, 4, 8, 16]
    # grids = []
    # for _ in range(16):
    #     grids.append([random.choice(nums) for _ in range(16)])
    grid = [0, 0, 0, 2,
            8, 0, 0, 2,
            2, 4, 2, 4,
            1024, 1024, 0, 8]
    # grid = [2, 4, 8, 16, 32, 0, 64, 256, 512, 1024, 2048, 0, 8192, 16384, 4, 4]
    degrees = [degree(x) for x in grid]
    grid_num = 0
    shift = 0
    for bin_num in degrees[::-1]:
        grid_num |= bin_num << shift
        shift += 4
    print(bin(grid_num))
    print(grid_num)
