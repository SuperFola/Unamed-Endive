import os
import glob
import time
import pygame
from const import *
import functions
import textentry
import historic


class Editor:
    @staticmethod
    def convert_layer_to_int(lay):
        if lay == "map":
            return 0
        elif lay == "map2":
            return 1
        elif lay == "map3":
            return 2

    @staticmethod
    def get_next_layer(lay, way=+1):
        if way == +1:
            if lay == "map":
                return "map2"
            elif lay == "map2":
                return "map3"
            elif lay == "map3":
                return "map"
        elif way == -1:
            if lay == "map":
                return "map3"
            elif lay == "map2":
                return "map"
            elif lay == "map3":
                return "map2"

    def __init__(self):
        self.done = False
        self.map_path = "%i-%s.umd" % (len(glob.glob("maps/*")), time.strftime("%H%M"))
        self.layer = "map"
        self.tmap = functions.create_default()
        self.current_block = 0
        self.clic = 0  # 0 = not clicking ; 1 = left clicking ; 2 = right clicking ; 3 = wheel clicking
        self.alpha_black, self.win, self.tiles, self.tileset, self.font = None, None, None, None, None
        self.texts = []
        self.display_tb = True
        self.tb = []
        self.entry = None
        self.tiles_used = []
        self.tilesused_offset = 0
        self.hist = historic.Historic()
        self.scroll_ts = 0

    def resize_tmap(self, action):
        action = list(action)
        action[3] = int(action[3])
        to_change = []
        ow = self.tmap["width"]
        oh = self.tmap["height"]
        nw = ow
        nh = oh

        if action[0] == "w":
            if action[2] == "+":
                nw += action[3]
            if action[2] == "-":
                nw -= action[3]
        elif action[0] == "h":
            if action[2] == "+":
                nh += action[3]
            if action[2] == "-":
                nh -= action[3]

        for y, line in enumerate(self.tmap["map"]):
            for x, case in enumerate(line):
                if action[0] == "w":
                    if action[1] == "r":
                        if x == len(line) - 1:
                            to_change.append((x, y))
                    elif action[1] == "l":
                        if x == 0:
                            to_change.append((x, y))
                elif action[0] == "h":
                    if action[1] == "t":
                        if y == 0:
                            to_change.append((x, y))
                    elif action[1] == "b":
                        if y == len(self.tmap["map"]) - 1:
                            to_change.append((x, y))

        if action[2] == "+":
            # insert
            for (x, y) in to_change[::-1]:
                _x = x
                _y = y
                if action[0] == "w" and action[1] == "r":
                    _x += 1
                if action[0] == "h" and action[1] == "b":
                    _y += 1
                if action[0] == "w":
                    self.tmap["map"][_y].insert(_x, {"id": None, "colliding": False})
                    self.tmap["map2"][_y].insert(_x, {"id": None, "colliding": False})
                    self.tmap["map3"][_y].insert(_x, {"id": None, "colliding": False})
                else:
                    self.tmap["map"].insert(_y, [{"id": None, "colliding": False} for _ in range(self.tmap["width"])])
                    self.tmap["map2"].insert(_y, [{"id": None, "colliding": False} for _ in range(self.tmap["width"])])
                    self.tmap["map3"].insert(_y, [{"id": None, "colliding": False} for _ in range(self.tmap["width"])])
                    break
        elif action[2] == "-":
            # pop
            for (x, y) in to_change[::-1]:
                if action[0] == "w":
                    self.tmap["map"][y].pop(x)
                    self.tmap["map2"][y].pop(x)
                    self.tmap["map3"][y].pop(x)
                else:
                    self.tmap["map"].pop(y)
                    self.tmap["map2"].pop(y)
                    self.tmap["map3"].pop(y)
                    break

        self.tmap["width"] = nw
        self.tmap["height"] = nh

    def set(self, var, val):
        if var == "tilesused_offset":
            self.tilesused_offset = val

    def load(self, temp):
        if temp:
            if temp["type"] == "map":
                self.map_path = temp["path"].split('/')[-1]
                print(self.map_path)
                self.tmap = temp["content"]
            if temp["type"] == "new map":
                self.tmap["width"] = temp.get("width", DEF_WIDTH)
                self.tmap["height"] = temp.get("height", DEF_HEIGHT)
                print("Creating a map of %ix%i" % (self.tmap["width"], self.tmap["height"]))

        if not temp or temp["type"] == "new map":
            self.tmap["map"] = functions.convert_monolay_to_multilay([{"id": 0, "colliding": False} for _ in range(self.tmap["width"] * self.tmap["height"])], self.tmap["width"])
            self.tmap["map2"] = functions.convert_monolay_to_multilay([{"id": 0, "colliding": False} for _ in range(self.tmap["width"] * self.tmap["height"])], self.tmap["width"])
            self.tmap["map3"] = functions.convert_monolay_to_multilay([{"id": 0, "colliding": False} for _ in range(self.tmap["width"] * self.tmap["height"])], self.tmap["width"])
            self.tmap["tp"] = {}

        self.tiles_used = [0]

        self.win = pygame.display.set_mode(WSIZE)
        pygame.display.set_caption(TITLE)
        
        self.entry = textentry.TextBox(self.win, x=(W - 120) // 2, y=20, sx=240, bgcolor=(128, 128, 128))

        self.alpha_black = pygame.Surface((16, 16))
        self.alpha_black.fill(0)
        self.alpha_black.convert_alpha()
        self.alpha_black.set_alpha(90)

        print("Loading tileset")
        self.tileset = pygame.image.load("tileset.png").convert()
        print("Cutting in tiles of %i*%i" % (REAL_TS, REAL_TS))
        self.tiles = functions.cut_ts_in_tiles(self.tileset)
        print("Successfully loaded the tiles")

        self.font = pygame.font.SysFont("arial", 18)
        self.texts.append(self.font.render("layer 0", True, WHITE))
        self.texts.append(self.font.render("layer 1", True, WHITE))
        self.texts.append(self.font.render("layer 2", True, WHITE))

        self.tb.append(["set all to collide",   lambda: (self.hist.add(self.tmap), self._make_all_colliding())])
        self.tb.append(["unset all to collide", lambda: (self.hist.add(self.tmap), self._make_all_uncolliding)])
        self.tb.append(["next layer",           lambda: (self.hist.add(self.tmap), self.change_layer(+1))])
        self.tb.append(["previous layer",       lambda: (self.hist.add(self.tmap), self.change_layer(-1))])
        self.tb.append(["load",                 lambda: (self.hist.add(self.tmap), self.loadmap)])
        self.tb.append(["save",                 lambda: (self.hist.add(self.tmap), self.save)])
        self.tb.append(["width + 1 (right)",    lambda: (self.hist.add(self.tmap), self.resize_tmap("wr+1"))])
        self.tb.append(["width + 1 (left)",     lambda: (self.hist.add(self.tmap), self.resize_tmap("wl+1"))])
        self.tb.append(["width - 1 (right)",    lambda: (self.hist.add(self.tmap), self.resize_tmap("wr-1"))])
        self.tb.append(["width - 1 (left)",     lambda: (self.hist.add(self.tmap), self.resize_tmap("wl-1"))])
        self.tb.append(["height + 1 (top)",     lambda: (self.hist.add(self.tmap), self.resize_tmap("ht+1"))])
        self.tb.append(["height + 1 (bottom)",  lambda: (self.hist.add(self.tmap), self.resize_tmap("hb+1"))])
        self.tb.append(["height - 1 (top)",     lambda: (self.hist.add(self.tmap), self.resize_tmap("ht-1"))])
        self.tb.append(["height - 1 (bottom)",  lambda: (self.hist.add(self.tmap), self.resize_tmap("hb-1"))])
        self.tb.append(["< tiles used",         lambda: (self.hist.add(self.tmap), self.set("tilesused_offset", self.tilesused_offset - 1))])
        self.tb.append(["tiles used >",         lambda: (self.hist.add(self.tmap), self.set("tilesused_offset", self.tilesused_offset + 1))])

        for i, e in enumerate(self.tb):
            self.tb[i][0] = self.font.render(e[0], True, BLACK)
            self.tb[i].append(COLORS[i % len(COLORS)])

    def _make_all_colliding(self):
        for line in self.tmap[self.layer]:
            for case in line:
                case["colliding"] = True

    def _make_all_uncolliding(self):
        for line in self.tmap[self.layer]:
            for case in line:
                case["colliding"] = False

    def render_toolbox(self):
        pygame.draw.rect(self.win, GREY, (W - 200, 0, 200, H))

        for i, e in enumerate(self.tb):
            pygame.draw.rect(self.win, self.tb[i][2], (W - 200, i * 20, 200, 20))
            self.win.blit(self.tb[i][0], (W - 180, i * 20))

    def render_tileset(self):
        for i, e in enumerate(self.tiles):
            if self.scroll_ts <= i // 9 <= self.scroll_ts + H // REAL_TS:
                self.win.blit(e, (W - 144 + (i % 9) * REAL_TS, (i // 9 - self.scroll_ts) * REAL_TS))

    def render(self):
        # background
        pygame.draw.rect(self.win, (0, 0, 0), (0, 0) + self.win.get_size())
        # a background to visualize mapping errors
        pygame.draw.rect(self.win, PURPLE, (0, 0, self.tmap["width"] * REAL_TS, self.tmap["height"] * REAL_TS))

        try:
            tps = {i["fromcase"]: i["tomap"] for i in self.tmap["tp"]}
        except KeyError:
            tps = {}

        i = 0
        for y, line in enumerate(self.tmap[self.layer]):
            for x, element in enumerate(line):
                if element["id"] is not None:
                    self.win.blit(self.tiles[element["id"]], (x * REAL_TS, y * REAL_TS))
                    # if the block is not colliding, let's draw a semi black thing on it
                    # to help the mapper identifying them (the non colliding tiles)
                    if not element["colliding"]:
                        self.win.blit(self.alpha_black, (x * REAL_TS, y * REAL_TS))
                    # if the bloc is a TP, we put a small indication on it
                    if i in tps.keys():
                        self.win.blit(self.font.render("-" + str(tps[i]) + "-", True, (0, 0, 0)), (x * REAL_TS, y * REAL_TS))
                i += 1
        self.win.blit(self.texts[Editor.convert_layer_to_int(self.layer)],
                      (10, self.tmap["height"] * REAL_TS + 10))

        if self.display_tb:
            self.render_toolbox()
        else:
            self.render_tileset()

        # current block
        xp, yp = pygame.mouse.get_pos()
        xp += 10
        yp += 10
        self.win.blit(self.tiles[self.current_block], (xp, yp))

        # displaying tiles used on the map
        for i, t in enumerate(self.tiles_used):
            if self.tilesused_offset <= i <= self.tilesused_offset + (W // REAL_TS):
                self.win.blit(self.tiles[t], ((i - self.tilesused_offset) * TS, H - TS))

    def loadmap(self):
        self.hist.empty()

        self.entry.reset()
        self.entry.set_placeholder("map path")
        self.map_path = self.entry.get_text()

        while not os.path.exists("maps/{}".format(self.map_path)) and not os.path.exists("maps/{}.umd".format(self.map_path)):
            self.entry.reset()
            self.entry.set_placeholder("map path ?")
            self.map_path = self.entry.get_text()

        with open("maps/{}".format(self.map_path)) as file:
            self.tmap = eval(file.read().replace('null', 'None').replace('false', 'False').replace('true', 'True'))

        self.tiles_used = functions.get_tiles_used_on_map(self.tmap)

        self.tmap["map"] = functions.convert_monolay_to_multilay(self.tmap["map"], self.tmap["width"])
        self.tmap["map2"] = functions.convert_monolay_to_multilay(self.tmap["map2"], self.tmap["width"])
        self.tmap["map3"] = functions.convert_monolay_to_multilay(self.tmap["map3"], self.tmap["width"])

    def save(self):
        self.entry.reset()
        self.entry.set_placeholder("map path")
        self.map_path = self.entry.get_text()

        # to save in the decent format
        self.tmap["map"] = functions.convert_multilay_to_monolay(self.tmap["map"])
        self.tmap["map2"] = functions.convert_multilay_to_monolay(self.tmap["map2"])
        self.tmap["map3"] = functions.convert_multilay_to_monolay(self.tmap["map3"])

        with open("maps/%s" % self.map_path, "w") as file:
            file.write(str(self.tmap).replace("'", '"').replace('True', "true").replace('False', "false"))
        print("Saved to maps/%s" % self.map_path)

        # putting back the data correctly
        self.tmap["map"] = functions.convert_monolay_to_multilay(self.tmap["map"], self.tmap["width"])
        self.tmap["map2"] = functions.convert_monolay_to_multilay(self.tmap["map2"], self.tmap["width"])
        self.tmap["map3"] = functions.convert_monolay_to_multilay(self.tmap["map3"], self.tmap["width"])

    def change_collide_state(self, v):
        xp, yp = pygame.mouse.get_pos()
        self.tmap[self.layer][yp // REAL_TS][xp // REAL_TS]["colliding"] = v

    def change_layer(self, v):
        if v == +1:
            if self.layer == "map2":
                self.layer = "map"
            elif self.layer == "map3":
                self.layer = "map2"
            elif self.layer == "map":
                self.layer = "map3"
        elif v == -1:
            if self.layer == "map3":
                self.layer = "map"
            elif self.layer == "map2":
                self.layer = "map3"
            elif self.layer == "map":
                self.layer = "map2"

    def add_tp(self):
        self.hist.add(self.tmap)

        xp, yp = pygame.mouse.get_pos()
        rpos = xp // REAL_TS + yp // REAL_TS * self.tmap["width"]

        tdel = -1
        for i, e in enumerate(self.tmap["tp"]):
            if e["fromcase"] == rpos:
                tdel = i
                break
        if tdel != -1:
            self.tmap["tp"].pop(tdel)
            return True

        self.entry.reset()
        self.entry.set_placeholder("tomap(int)")
        tomap = self.entry.get_text()
        try:
            tomap = int(tomap)
        except ValueError:
            print("Need an integer")
        else:
            self.entry.reset()
            self.entry.set_placeholder("tocase(int)")
            tocase = self.entry.get_text()
            try:
                tocase = int(tocase)
            except ValueError:
                print("Need an integer")
            else:
                self.tmap["tp"].append({"fromcase": rpos, "tomap": tomap, "tocase": tocase})

    def process_ev(self, event):
        allkeys = pygame.key.get_pressed()

        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            self.done = True
        elif allkeys[pygame.K_w] and (allkeys[pygame.K_LCTRL] or allkeys[pygame.K_RCTRL]):
            self.tmap = self.hist.set(self.hist.current - 1)
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_s:
                # save the map
                self.save()
            elif event.key == pygame.K_l:
                # load a map
                self.loadmap()
            elif event.key == pygame.K_c:
                # change the colliding state
                self.hist.add(self.tmap)
                self.change_collide_state(True)
            elif event.key == pygame.K_v:
                # change the colliding state
                self.hist.add(self.tmap)
                self.change_collide_state(False)
            elif event.key == pygame.K_UP or event.key == pygame.K_RIGHT:
                # change the layer
                self.change_layer(+1)
            elif event.key == pygame.K_DOWN or event.key == pygame.K_LEFT:
                # change the layer
                self.change_layer(-1)
            elif event.key == pygame.K_h:
                # display or not the toolbox
                self.display_tb = not self.display_tb
            elif event.key == pygame.K_p:
                # adding tp
                self.add_tp()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button < 4:
                self.clic = event.button
            elif event.button == 4:
                # mouse wheel going up
                self.scroll_ts += 1
            elif event.button == 5:
                # going down
                self.scroll_ts -= 1

            if 1 <= self.clic < 4:
                self.hist.add(self.tmap)

            xp, yp = event.pos
            lay = self.layer

            if self.clic == 1 and yp >= H - TS:
                rx = xp // TS + self.tilesused_offset
                if 0 <= rx < len(self.tiles_used):
                    self.current_block = self.tiles_used[rx]

            if not self.display_tb and xp >= W - 144:
                rx = (xp - W + 144) // REAL_TS
                ry = yp // REAL_TS + self.scroll_ts
                if 0 <= rx <= 9 and 0 <= ry <= len(self.tiles):
                    rs = rx + ry * 9
                    self.current_block = rs
            elif ((self.display_tb and xp < W - 200) or not self.display_tb) and 0 <= yp // REAL_TS < len(self.tmap[lay]) \
                    and 0 <= xp // REAL_TS < len(self.tmap[lay][yp // REAL_TS]):
                if self.clic == 1:
                    # put a block
                    self.tmap[lay][yp // REAL_TS][xp // REAL_TS]["id"] = self.current_block
                    if self.current_block not in self.tiles_used:
                        self.tiles_used.append(self.current_block)
                elif self.clic == 2:
                    # collide state
                    self.tmap[lay][yp // REAL_TS][xp // REAL_TS]["colliding"] = not self.tmap[lay][yp // REAL_TS][xp // REAL_TS]["colliding"]
                elif self.clic == 3:
                    # put a tp or delete one
                    self.add_tp()
            elif self.display_tb and xp >= W - 200:
                # we clicked in the toolbox
                ry = yp // 20
                if 0 <= ry < len(self.tb):
                    self.tb[ry][1]() if self.tb[ry][1] else None
        elif event.type == pygame.MOUSEMOTION:
            xp, yp = event.pos
            lay = self.layer

            if xp // REAL_TS < 0 or yp // REAL_TS < 0 or yp // REAL_TS >= len(self.tmap[lay]) or xp // REAL_TS >= len(self.tmap[lay][0]):
                self.clic = 0

            if ((self.display_tb and xp < W - 200) or not self.display_tb) and 0 <= yp // REAL_TS < len(self.tmap[lay]) \
                    and 0 <= xp // REAL_TS < len(self.tmap[lay][yp // REAL_TS]):
                if self.clic == 1:
                    # put a block
                    if self.tmap[lay][yp // REAL_TS][xp // REAL_TS]["id"] != self.current_block:
                        self.hist.add(self.tmap)
                    self.tmap[lay][yp // REAL_TS][xp // REAL_TS]["id"] = self.current_block
                    if self.current_block not in self.tiles_used:
                        self.tiles_used.append(self.current_block)
                elif self.clic == 2:
                    # collide state
                    self.tmap[lay][yp // REAL_TS][xp // REAL_TS]["colliding"] = not self.tmap[lay][yp // REAL_TS][xp // REAL_TS]["colliding"]
        elif event.type == pygame.MOUSEBUTTONUP:
            self.clic = 0

    def run(self, temp):
        self.load(temp)
        print("Controls :" + "\n\t- ".join(controls))

        while not self.done:
            # events
            for event in pygame.event.get():
                self.process_ev(event)
            # tilemap
            self.render()
            # flip
            pygame.display.flip()