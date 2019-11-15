def degree(num):
    count = 0
    while num > 1:
        num >>= 1
        count += 1
    return count


if __name__ == '__main__':
    grid = [64, 0, 0, 0, 4, 2, 16, 16, 2, 2, 4, 4, 64, 2, 2, 2]
    degrees = [degree(x) for x in grid]
    grid_num = shift = 0
    for i, bin_num in enumerate(degrees[::-1]):
        grid_num |= bin_num << shift
        shift += 4
    print(grid)
    print(degrees)
    print(grid_num)
