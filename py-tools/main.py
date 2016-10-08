# maps editor (using tileset and not separate tiles in the assets/tiles folder)
# made by Hxokunlug on 27th, August, 2016
# All rights reserved

import sys


TS = 17
REAL_TS = 16
DEF_WIDTH = 16
DEF_HEIGHT = 8
VERSION = "v0.0.3"

print("Starting")

temp = None

if sys.argv[1:]:
    print("Got argument(s) from command line.")
    if "-p" in sys.argv[1:]:
        try:
            map_path_to_load = sys.argv[1:][sys.argv[1:].index("-p") + 1]
            print("Loading a map at %s" % map_path_to_load)
            temp = {
                "type": "map",
                "path": map_path_to_load,
                "content": eval(open(map_path_to_load).read())
            }
        except IndexError:
            print("Missing argument for option -p")
    if "-W" in sys.argv[1:]:
        try:
            width = sys.argv[1:][sys.argv[1:].index("-W") + 1]
            if not temp:
                temp = {
                    "type": "new map"
                }
                temp["width"] = int(width)
            elif temp["type"] == "new map":
                temp["width"] = int(width)
            else:
                print("You can not specify a default width for an existing map")
        except IndexError or ValueError:
            print("Missing argument for option -W")
    if "-H" in sys.argv[1:]:
        try:
            height = sys.argv[1:][sys.argv[1:].index("-H") + 1]
            if not temp:
                temp = {
                    "type": "new map"
                }
                temp["height"] = int(height)
            elif temp["type"] == "new map":
                temp["height"] = int(height)
            else:
                print("You can not specify a default height for an existing map")
        except IndexError or ValueError:
            print("Missing argument for option -H")
    if "-v" in sys.argv[1:]:
        print(VERSION)
        sys.exit(0)
    if "-h" in sys.argv[1:]:
        print("""Available options :
        -h => display this help
        -v => display editor version
        -p [path] => indicate to the editor which map to load
        -W [width] => specify a default width
        -H [height] => specify a default height
        """)
        sys.exit(0)


import os
import glob
import time
import pygame


pygame.init()
win = pygame.display.set_mode((840, 540))
pygame.display.set_caption("Unamed Maps Editor (Py3 version)")

if not os.path.exists("maps"):
    os.mkdir("maps")
map_path = "%i-%s.umd" % (len(glob.glob("maps/*")), time.strftime("%H%M"))


def cut_ts_in_tiles(ts):
    list_tiles = []

    for y in range(ts.get_height() // TS):
        for x in range(ts.get_width() // TS):
            tu, tv = x * TS, y * TS
            list_tiles.append(ts.subsurface((tu, tv, REAL_TS, REAL_TS)))
    print("Loaded %i tiles" % len(list_tiles))
    return list_tiles


def render(screen, tilemap, layer, list_tiles):
    # a Background to visualize mapping errors
    pygame.draw.rect(screen, (255, 0, 0), (0, 0, tilemap["width"] * REAL_TS, tilemap["height"] * REAL_TS))

    for i, element in enumerate(tilemap[layer]):
        if element is not None:
            tu, tv = i % tilemap["width"], i // tilemap["width"]
            screen.blit(list_tiles[element["id"]], (tu * REAL_TS, tv * REAL_TS))


def resize_tmap(tmap, nw, nh):
    to_change = []
    ow = tmap["width"]
    oh = tmap["height"]
    for i, element in enumerate(tmap):
        tu, tv = i % ow, i // ow
        if (ow != nw ^ oh != nw) and ((tu == nw & ow != nw) ^ (tv == nh & oh != nh)):
            to_change.append(i)

    to_change = to_change[::-1]

    if ow < nw ^ oh < nh:
        # insert
        for elem in to_change:
            tmap["map"].insert(elem, None)
            tmap["map2"].insert(elem, None)
            tmap["map3"].insert(elem, None)
    elif ow > nw ^ oh > nh:
        # pop
        for elem in to_change:
            tmap["map"].pop(elem)
            tmap["map2"].pop(elem)
            tmap["map3"].pop(elem)


print("Loading tileset")
tileset = pygame.image.load("../assets/tileset.png").convert()
print("Cutting in tiles of %i*%i" % (REAL_TS, REAL_TS))
tiles = cut_ts_in_tiles(tileset)
print("Succesfully loaded the tiles")

tmap = {
    "map": [],
    "map2": [],
    "map3": [],
    "width": DEF_WIDTH,
    "height": DEF_HEIGHT
}
done = False
current_block = 0
clic = 0  # 0 = not clicking ; 1 = left clicking ; 2 = right clicking ; 3 = wheel clicking

if temp:
    if temp["type"] == "map":
        map_path = temp["path"].split('/')[-1]
        print(map_path)
        tmap = temp["content"]
        temp = None
    if temp["type"] == "new map":
        tmap["width"] = temp.get("width", DEF_WIDTH)
        tmap["height"] = temp.get("height", DEF_HEIGHT)
        print("Creating a map of %ix%i" % (tmap["width"], tmap["height"]))

tmap["map"] = [{"id": 0, "colliding": False} for _ in range(tmap["width"] * tmap["height"])]
tmap["map2"] = [{"id": 0, "colliding": False} for _ in range(tmap["width"] * tmap["height"])]
tmap["map3"] = [{"id": 0, "colliding": False} for _ in range(tmap["width"] * tmap["height"])]
layer = "map"

controls = [
    "KP7 : map width + 1",
    "KP8 : map width - 1",
    "KP4 : map height + 1",
    "KP5 : map height - 1",
    "s : save the map",
    "c : the block under the cursor is now colliding",
    "v : the block under the cursor is no longer colliding",
    "left clic : put a block",
    "middle clic : pick a bloc",
    "right clic : destroy a block",
    "up : change the layer",
    "down : change the layer"
]

print("Controls :" + "\n\t- ".join(controls))

while not done:
    for event in pygame.event.get():
        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            done = True
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_KP7:
                # width + 1
                resize_tmap(tmap, tmap["width"] + 1, tmap["height"])
                tmap["width"] += 1
            elif event.key == pygame.K_KP8:
                # width - 1
                resize_tmap(tmap, tmap["width"] - 1, tmap["height"])
                tmap["width"] -= 1
            elif event.key == pygame.K_KP4:
                # height + 1
                resize_tmap(tmap, tmap["width"], tmap["height"] + 1)
                tmap["height"] += 1
            elif event.key == pygame.K_KP5:
                # height - 1
                resize_tmap(tmap, tmap["width"], tmap["height"] - 1)
                tmap["height"] -= 1
            elif event.key == pygame.K_s:
                # save the map
                with open("maps/%s" % map_path, "w") as file:
                    file.write(str(tmap).replace("'", '"').replace('True', "true").replace('False', "false"))
                print("Saved to maps/%s" % map_path)
            elif event.key == pygame.K_c:
                # change the colliding state
                xp, yp = pygame.mouse.get_pos()
                rpos = xp // REAL_TS + yp // REAL_TS * tmap["width"]
                tmap[layer][rpos]["colliding"] = True
                print("Block at %s (%i) is colliding" % (rpos, tmap[layer][rpos]["id"]))
            elif event.key == pygame.K_v:
                # change the colliding state
                xp, yp = pygame.mouse.get_pos()
                rpos = xp // REAL_TS + yp // REAL_TS * tmap["width"]
                tmap[layer][rpos]["colliding"] = False
                print("Block at %s (%i) is not colliding" % (rpos, tmap[layer][rpos]["id"]))
            elif event.key == pygame.K_UP:
                # change the layer
                print("Layer %s" % layer)
                if layer == "map2":
                    layer = "map"
                elif layer == "map3":
                    layer = "map2"
                elif layer == "map":
                    layer = "map3"
                print("New layer is %s" % layer)
            elif event.key == pygame.K_DOWN:
                # change the layer
                print("Layer %s" % layer)
                if layer == "map3":
                    layer = "map"
                elif layer == "map2":
                    layer = "map3"
                elif layer == "map":
                    layer = "map2"
                print("New layer is %s" % layer)
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button < 4:
                clic = event.button
            elif event.button == 4:
                # mouse wheel going up
                current_block += 1
                if current_block >= len(tiles):
                    current_block = 0
            elif event.button == 5:
                # going down
                current_block -= 1
                if current_block < 0:
                    current_block = len(tiles) - 1

            xp, yp = event.pos
            rpos = xp // REAL_TS + yp // REAL_TS * tmap["width"]

            if clic == 1:
                # put a block
                tmap[layer][rpos]["id"] = current_block
            elif clic == 2:
                # destroy a block
                tmap[layer][rpos]["id"] = None
            elif clic == 3:
                # pick a chu ... pick a block
                current_block = tmap[layer][rpos]["id"]
        elif event.type == pygame.MOUSEMOTION:
            xp, yp = event.pos
            rpos = xp // REAL_TS + yp // REAL_TS * tmap["width"]

            if rpos < 0 or rpos >= len(tmap[layer]):
                clic = 0

            if clic == 1:
                # put a block
                tmap[layer][rpos]["id"] = current_block
            elif clic == 2:
                # destroy a block
                tmap[layer][rpos]["id"] = None
            elif clic == 3:
                # pick a chu ... pick a block
                current_block = tmap[layer][rpos]["id"]
        elif event.type == pygame.MOUSEBUTTONUP:
            clic = 0

    pygame.draw.rect(win, (0, 0, 0), (0, 0) + win.get_size())
    render(win, tmap, layer, tiles)
    xp, yp = pygame.mouse.get_pos()
    xp += 10 ; yp += 10
    win.blit(tiles[current_block], (xp, yp))
    pygame.display.flip()

print("Quitting")
pygame.quit()
