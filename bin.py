def degree(num):
    count = 0
    while num > 1:
        num >>= 1
        count += 1
    return count


if __name__ == '__main__':
    grid = [0, 2, 0, 2,
            2, 2, 2, 0,
            0, 0, 0, 2,
            0, 0, 2, 0]
    # grid = [2, 4, 8, 16, 32, 64, 64, 256, 512, 1024, 2048, 4096, 8192, 16384, 4, 4]
    degrees = [degree(x) for x in grid]
    grid_num = shift = 0
    for i, bin_num in enumerate(degrees[::-1]):
        grid_num |= bin_num << shift
        shift += 4
    print(grid)
    print(degrees)
    print((grid_num))

