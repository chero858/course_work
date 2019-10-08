import pyautogui
import cv2


def tile_match(tile1, tile2):
    original = cv2.imread(tile1)
    image_to_compare = cv2.imread(tile2)

    sift = cv2.xfeatures2d.SIFT_create()
    kp_1, desc_1 = sift.detectAndCompute(original, None)
    kp_2, desc_2 = sift.detectAndCompute(image_to_compare, None)
    index_params = dict(algorithm=0, trees=5)
    search_params = dict()
    flann = cv2.FlannBasedMatcher(index_params, search_params)
    matches = flann.knnMatch(desc_1, desc_2, k=2)
    good_points = 0
    ratio = 0.6
    for m, n in matches:
        if m.distance < ratio * n.distance:
            good_points += 1

    return True if good_points >= 10 else False # 10 потому что у одинаковых тайлов примерно минимум 10 точек совпаднения


def get_tile(tile):
    cell = 2
    while cell <= 2048:
        if tile_match(tile, 'tiles_image/' + str(cell) + '.png'):
            return cell
        cell *= 2

    return 0


def get_grid():
    grid = []
    x = 489
    y = 330
    for row in range(4):
        for cell in range(4):
            pyautogui.screenshot('/home/roman/PycharmProjects/course_work/tile.png', region=(x, y, 85, 85))
            grid.append(get_tile('tile.png'))
            x += 97
        y += 97
        x = 489

    return grid


def main():
    print(get_grid())


if __name__ == '__main__':
    main()
