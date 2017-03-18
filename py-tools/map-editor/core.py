import glob
import time
import pygame
from const import *
import functions
import textentry


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

    def resize_tmap(self, nw, nh):
        to_change = []
        ow = self.tmap["width"]
        oh = self.tmap["height"]

        for i, element in enumerate(self.tmap["map"]):
            tu, tv = i % ow, i // ow

            if ow < nw and tu == 0:
                to_change.append(i)
            elif ow > nw and tu == nw:
                to_change.append(i)
            elif oh < nh and tv == 0:
                to_change.append(i)
            elif oh > nh and tv == nh:
                to_change.append(i)

        if (ow < nw) or (oh < nh):
            # insert
            print("insert", len(to_change))
            for elem in to_change[::-1]:
                self.tmap["map"].insert(elem, None)
                self.tmap["map2"].insert(elem, None)
                self.tmap["map3"].insert(elem, None)
        elif (ow > nw) or (oh > nh):
            # pop
            print("pop", len(to_change))
            for elem in to_change[::-1]:
                self.tmap["map"].pop(elem)
                self.tmap["map2"].pop(elem)
                self.tmap["map3"].pop(elem)
        else:
            print("else. should not be here")

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
            self.tmap["map"] = [{"id": 0, "colliding": False} for _ in range(self.tmap["width"] * self.tmap["height"])]
            self.tmap["map2"] = [{"id": 0, "colliding": False} for _ in range(self.tmap["width"] * self.tmap["height"])]
            self.tmap["map3"] = [{"id": 0, "colliding": False} for _ in range(self.tmap["width"] * self.tmap["height"])]

        self.tiles_used = functions.get_tiles_used_on_map(self.tmap)

        self.win = pygame.display.set_mode(WSIZE)
        pygame.display.set_caption(TITLE)
        
        self.entry = textentry.TextBox(self.win, x=(W - 120) // 2, y=20, sx=240, bgcolor=(128, 128, 128))

        self.alpha_black = pygame.Surface((16, 16))
        self.alpha_black.fill(0)
        self.alpha_black.convert_alpha()
        self.alpha_black.set_alpha(90)

        print("Loading tileset")
        self.tileset = pygame.image.load("../../assets/tileset.png").convert()
        print("Cutting in tiles of %i*%i" % (REAL_TS, REAL_TS))
        self.tiles = functions.cut_ts_in_tiles(self.tileset)
        print("Successfully loaded the tiles")

        self.font = pygame.font.SysFont("arial", 18)
        self.texts.append(self.font.render("layer 0", True, WHITE))
        self.texts.append(self.font.render("layer 1", True, WHITE))
        self.texts.append(self.font.render("layer 2", True, WHITE))

        self.tb.append(["set all to collide", self._make_all_colliding])
        self.tb.append(["unset all to collide", self._make_all_uncolliding])
        self.tb.append(["next layer", lambda: self.change_layer(+1)])
        self.tb.append(["previous layer", lambda: self.change_layer(-1)])
        self.tb.append(["load", self.loadmap])
        self.tb.append(["save", self.save])
        self.tb.append(["width + 1", lambda: self.resize_tmap(self.tmap["width"] + 1, self.tmap["height"])])
        self.tb.append(["width - 1", lambda: self.resize_tmap(self.tmap["width"] - 1, self.tmap["height"])])
        self.tb.append(["height + 1", lambda: self.resize_tmap(self.tmap["width"], self.tmap["height"] + 1)])
        self.tb.append(["height - 1", lambda: self.resize_tmap(self.tmap["width"], self.tmap["height"] - 1)])
        self.tb.append(["< tiles used", lambda: self.set("tilesused_offset", self.tilesused_offset - 1)])
        self.tb.append(["tiles used >", lambda: self.set("tilesused_offset", self.tilesused_offset + 1)])

        for i, e in enumerate(self.tb):
            self.tb[i][0] = self.font.render(e[0], True, BLACK)
            self.tb[i].append(COLORS[i % len(COLORS)])

    def _make_all_colliding(self):
        for elem in self.tmap[self.layer]:
            elem["colliding"] = True

    def _make_all_uncolliding(self):
        for elem in self.tmap[self.layer]:
            elem["colliding"] = False

    def render_toolbox(self):
        pygame.draw.rect(self.win, GREY, (W - 200, 0, 200, H))

        for i, e in enumerate(self.tb):
            pygame.draw.rect(self.win, self.tb[i][2], (W - 200, i * 20, 200, 20))
            self.win.blit(self.tb[i][0], (W - 180, i * 20))

    def render(self):
        # background
        pygame.draw.rect(self.win, (0, 0, 0), (0, 0) + self.win.get_size())
        # a background to visualize mapping errors
        pygame.draw.rect(self.win, PURPLE, (0, 0, self.tmap["width"] * REAL_TS, self.tmap["height"] * REAL_TS))

        tps = {i["fromcase"]: i["tomap"] for i in self.tmap["tp"]}

        for i, element in enumerate(self.tmap[self.layer]):
            if element is not None:
                tu, tv = i % self.tmap["width"], i // self.tmap["width"]
                self.win.blit(self.tiles[element["id"]], (tu * REAL_TS, tv * REAL_TS))
                # if the block is not colliding, let's draw a semi black thing on it
                # to help the mapper identifying them (the non colliding tiles)
                if not element["colliding"]:
                    self.win.blit(self.alpha_black, (tu * REAL_TS, tv * REAL_TS))
                # if the bloc is a TP, we put a small indication on it
                if i in tps.keys():
                    self.win.blit(self.font.render("-" + str(tps[i]) + "-", True, (0, 0, 0)), (tu * REAL_TS, tv * REAL_TS))
        self.win.blit(self.texts[Editor.convert_layer_to_int(self.layer)],
                      (10, self.tmap["height"] * REAL_TS + 10))

        if self.display_tb:
            self.render_toolbox()

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
        self.entry.reset()
        self.entry.set_placeholder("map path")
        with open("maps/{}".format(self.entry.get_text())) as file:
            self.tmap = eval(file.read().replace('null', 'None').replace('false', 'False').replace('true', 'True'))
        self.tiles_used = functions.get_tiles_used_on_map(self.tmap)

    def save(self):
        with open("maps/%s" % self.map_path, "w") as file:
            file.write(str(self.tmap).replace("'", '"').replace('True', "true").replace('False', "false"))
        print("Saved to maps/%s" % self.map_path)

    def change_collide_state(self, v):
        xp, yp = pygame.mouse.get_pos()
        rpos = xp // REAL_TS + yp // REAL_TS * self.tmap["width"]
        self.tmap[self.layer][rpos]["colliding"] = v

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
        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            self.done = True
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_KP7:
                # width + 1
                self.resize_tmap(self.tmap["width"] + 1, self.tmap["height"])
            elif event.key == pygame.K_KP8:
                # width - 1
                self.resize_tmap(self.tmap["width"] - 1, self.tmap["height"])
            elif event.key == pygame.K_KP4:
                # height + 1
                self.resize_tmap(self.tmap["width"], self.tmap["height"] + 1)
            elif event.key == pygame.K_KP5:
                # height - 1
                self.resize_tmap(self.tmap["width"], self.tmap["height"] - 1)
            elif event.key == pygame.K_s:
                # save the map
                self.save()
            elif event.key == pygame.K_l:
                # load a map
                self.loadmap()
            elif event.key == pygame.K_c:
                # change the colliding state
                self.change_collide_state(True)
            elif event.key == pygame.K_v:
                # change the colliding state
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
                self.current_block += 1
                if self.current_block >= len(self.tiles):
                    self.current_block = 0
            elif event.button == 5:
                # going down
                self.current_block -= 1
                if self.current_block < 0:
                    self.current_block = len(self.tiles) - 1

            xp, yp = event.pos
            rpos = xp // REAL_TS + yp // REAL_TS * self.tmap["width"]
            lay = self.layer

            if self.clic == 1 and yp >= H - TS:
                rx = xp // TS + self.tilesused_offset
                if 0 <= rx < len(self.tiles_used):
                    self.current_block = self.tiles_used[rx]

            if ((self.display_tb and xp < W - 200) or not self.display_tb) and 0 <= rpos < len(self.tmap[lay]):
                if self.clic == 1:
                    # put a block
                    self.tmap[lay][rpos]["id"] = self.current_block
                    if self.current_block not in self.tiles_used:
                        self.tiles_used.append(self.current_block)
                elif self.clic == 2:
                    # collide state
                    self.tmap[lay][rpos]["colliding"] = not self.tmap[lay][rpos]["colliding"]
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
            rpos = xp // REAL_TS + yp // REAL_TS * self.tmap["width"]
            lay = self.layer

            if rpos < 0 or rpos >= len(self.tmap[lay]):
                self.clic = 0

            if ((self.display_tb and xp < W - 200) or not self.display_tb) and 0 <= rpos < len(self.tmap[lay]):  # no drag and drop in the toolbox
                if self.clic == 1:
                    # put a block
                    self.tmap[lay][rpos]["id"] = self.current_block
                    if self.current_block not in self.tiles_used:
                        self.tiles_used.append(self.current_block)
                elif self.clic == 2:
                    # collide state
                    self.tmap[lay][rpos]["colliding"] = not self.tmap[lay][rpos]["colliding"]
                elif self.clic == 3:
                    # put a tp or delete one
                    self.add_tp()
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