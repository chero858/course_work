from selenium import webdriver
# import pandas as pd
import time
import random
from example import find_best_move
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait


def degree(num):
    count = 0
    while num > 1:
        num >>= 1
        count += 1
    return count


def get_grid(driver):
    grid = []
    for i in range(1, 5):
        for j in range(1, 5):
            if len(driver.find_elements_by_css_selector(f".tile.tile-position-{j}-{i}")) > 0:
                try:
                    grid.append(int(driver.find_elements_by_css_selector(f".tile.tile-position-{j}-{i}")[-1].text))
                except Exception:
                    grid.append(0)
            else:
                grid.append(0)
    return grid


def print_grid(grid):
    print(*grid[:4])
    print(*grid[4:8])
    print(*grid[8:12])
    print(*grid[12:16])


def game(driver):
    keys = {0: Keys.ARROW_UP, 1: Keys.ARROW_LEFT, 2: Keys.ARROW_DOWN, 3: Keys.ARROW_RIGHT}
    time.sleep(3)
    total_moves = 0
    total_time = 0
    while len(driver.find_elements_by_css_selector(f".game-message.game-over")) == 0 or len(
            driver.find_elements_by_css_selector(f'.retry-button')) == 0:
        # print('############')
        grid = get_grid(driver)
        num = get_num(grid)
        start_time = time.time()
        direction = find_best_move(num)
        total_time += time.time() - start_time
        total_moves += 1
        # print_grid(grid)
        # print(f"direction: {direction}")
        driver.find_element_by_css_selector('body').send_keys(keys[direction])
        if len(driver.find_elements_by_css_selector(f'.game-message.game-won')) > 0:
            driver.find_elements_by_css_selector(f'.keep-playing-button')[-1].click()
    res = driver.find_elements_by_css_selector(".score-container")[-1].text.split('\n')[0]
    with open('results.txt', 'a') as f:
        f.write(str(max(get_grid(driver))) + ' ' + res + ' ' + str(total_time) + ' ' + str(total_moves) + '\n')
    driver.find_elements_by_css_selector(f'.retry-button')[-1].click()
    print('end')
    time.sleep(3)
    game(driver)


def get_num(grid):
    degrees = [degree(x) for x in grid]
    grid_num = shift = 0
    for i, bin_num in enumerate(degrees[::-1]):
        grid_num |= bin_num << shift
        shift += 4
    return grid_num


def playing():
    driver = webdriver.Chrome(executable_path=r'chromedriver.exe')
    driver.get('https://play2048.co/')
    game(driver)
    res = driver.find_elements_by_css_selector(".score-container")[-1].text
    with open('results.txt', 'a') as f:
        f.write(str(max(get_grid(driver))) + '\n')
    driver.close()
    return res


if __name__ == '__main__':
    for i in range(100):
        print(playing())
