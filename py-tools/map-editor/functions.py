import os
from const import *


def start():
    if not os.path.exists("maps"):
        os.mkdir("maps")


def cut_ts_in_tiles(ts):
    list_tiles = []

    for y in range(ts.get_height() // TS):
        for x in range(ts.get_width() // TS):
            tu, tv = x * TS, y * TS
            list_tiles.append(ts.subsurface((tu, tv, REAL_TS, REAL_TS)))
    print("Loaded %i tiles" % len(list_tiles))
    return list_tiles


def create_default():
    return {
        "map": [],
        "map2": [],
        "map3": [],
        "width": DEF_WIDTH,
        "height": DEF_HEIGHT
    }
