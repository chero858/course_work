from selenium import webdriver
# import pandas as pd
import time
import random
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.action_chains import ActionChains


def get_grid():
    grid = []
    for i in range(1, 5):
        for j in range(1, 5):
            if len(driver.find_elements_by_css_selector(f".tile.tile-position-{j}-{i}")) > 0:
                grid.append(int(driver.find_elements_by_css_selector(f".tile.tile-position-{j}-{i}")[-1].text))
            else:
                grid.append(0)
    return grid


def print_grid(grid):
    print(*grid[:4])
    print(*grid[4:8])
    print(*grid[8:12])
    print(*grid[12:16])


def game(driver):
    actions = ActionChains(driver)
    keys = {1: Keys.ARROW_DOWN, 2: Keys.ARROW_UP, 3: Keys.ARROW_LEFT, 4: Keys.ARROW_RIGHT}
    while len(driver.find_elements_by_css_selector(f".game-message.game-over")) == 0:
        print('############')
        grid = get_grid()
        print_grid(grid)
        actions.send_keys(keys[random.randint(1, 4)]).perform()
        time.sleep(2.5)


if __name__ == '__main__':
    driver = webdriver.Chrome(executable_path=r'C:/Users/Администратор/Desktop/chromedriver.exe')
    driver.get('https://play2048.co/')
    game(driver)
    driver.close()
