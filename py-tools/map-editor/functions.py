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
        "height": DEF_HEIGHT,
        "spawns": [],
        "tp": []
    }


def get_tiles_used_on_map(tmap):
    w = []
    for k in ('map', 'map2', 'map3'):
        for i in tmap[k]:
            if i["id"] not in w:
                w.append(i["id"])
    return w


def convert_monolay_to_multilay(tmap, w):
    ntmap = []

    for i, e in enumerate(tmap):
        if not ntmap or len(ntmap[-1]) == w:
            ntmap.append([e])
        elif len(ntmap[-1]) < w:
            ntmap[-1].append(e)

    return ntmap


def convert_multilay_to_monolay(tmap):
    ntmap = []

    for y, line in enumerate(tmap):
        for x, case in enumerate(line):
            ntmap.append(case)

    return ntmap