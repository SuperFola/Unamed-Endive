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

    def resize_tmap(self, nw, nh):
        to_change = []
        ow = self.tmap["width"]
        oh = self.tmap["height"]
        print(ow, nw, oh, nh)
        for i, element in enumerate(self.tmap):
            tu, tv = i % ow, i // ow
            if (ow != nw) ^ (oh != nh):
                if ((tu == nw - 1) & (ow != nw)) ^ ((tv == nh - 1) & (oh != nh)):
                    to_change.append(i + 1)

        if (ow < nw) ^ (oh < nh):
            # insert
            print("insert")
            for elem in to_change[::-1]:
                self.tmap["map"].insert(elem, None)
                self.tmap["map2"].insert(elem, None)
                self.tmap["map3"].insert(elem, None)
        elif (ow > nw) ^ (oh > nh):
            # pop
            print("pop")
            for elem in to_change[::-1]:
                self.tmap["map"].pop(elem)
                self.tmap["map2"].pop(elem)
                self.tmap["map3"].pop(elem)
        else:
            print("else")

        self.tmap["width"] = nw
        self.tmap["height"] = nh

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

        self.win = pygame.display.set_mode(WSIZE)
        pygame.display.set_caption(TITLE)
        
        self.entry = textentry.TextBox(self.win, x=(W - 120) // 2, y=20, sx=120)

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
        self.tb.append(["save", self.save])
        self.tb.append(["width + 1", lambda: self.resize_tmap(self.tmap["width"] + 1, self.tmap["height"])])
        self.tb.append(["width - 1", lambda: self.resize_tmap(self.tmap["width"] - 1, self.tmap["height"])])
        self.tb.append(["height + 1", lambda: self.resize_tmap(self.tmap["width"], self.tmap["height"] + 1)])
        self.tb.append(["height - 1", lambda: self.resize_tmap(self.tmap["width"], self.tmap["height"] - 1)])

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
        pygame.draw.rect(self.win, PURPLE, (0, 0, self.tmap["width"] * REAL_TS * 2, self.tmap["height"] * REAL_TS))

        for i, element in enumerate(self.tmap[self.layer]):
            if element is not None:
                tu, tv = i % self.tmap["width"], i // self.tmap["width"]
                self.win.blit(self.tiles[element["id"]], (tu * REAL_TS, tv * REAL_TS))
                # if the block is not colliding, let's draw a semi black thing on it
                # to help the mapper identifying them (the non colliding tiles)
                if not element["colliding"]:
                    self.win.blit(self.alpha_black, (tu * REAL_TS, tv * REAL_TS))
        self.win.blit(self.texts[Editor.convert_layer_to_int(self.layer)],
                      (10, self.tmap["height"] * REAL_TS + 10))

        for i, element in enumerate(self.tmap[Editor.get_next_layer(self.layer)]):
            if element is not None:
                tu, tv = i % self.tmap["width"], i // self.tmap["width"]
                self.win.blit(self.tiles[element["id"]], (tu * REAL_TS + self.tmap["width"] * REAL_TS, tv * REAL_TS))
                # if the block is not colliding, let's draw a semi black thing on it
                # to help the mapper identifying them (the non colliding tiles)
                if not element["colliding"]:
                    self.win.blit(self.alpha_black, (tu * REAL_TS + self.tmap["width"] * REAL_TS, tv * REAL_TS))
        self.win.blit(self.texts[Editor.convert_layer_to_int(Editor.get_next_layer(self.layer))],
                      (10 + self.tmap["width"] * REAL_TS, self.tmap["height"] * REAL_TS + 10))

        pygame.draw.line(self.win, RED,
                         (self.tmap["width"] * REAL_TS, 0),
                         (self.tmap["width"] * REAL_TS, self.tmap["height"] * REAL_TS),
                         2)

        if self.display_tb:
            self.render_toolbox()

        # current block
        xp, yp = pygame.mouse.get_pos()
        xp += 10
        yp += 10
        self.win.blit(self.tiles[self.current_block], (xp, yp))

    def save(self):
        with open("maps/%s" % self.map_path, "w") as file:
            file.write(str(self.tmap).replace("'", '"').replace('True', "true").replace('False', "false"))
        print("Saved to maps/%s" % self.map_path)

    def change_collide_state(self, v):
        xp, yp = pygame.mouse.get_pos()
        rpos = xp // REAL_TS + yp // REAL_TS * self.tmap["width"]
        self.tmap[self.layer][rpos]["colliding"] = v
        print("Block at %s (%i) is colliding" % (rpos, self.tmap[self.layer][rpos]["id"]))

    def change_layer(self, v):
        print("Layer %s" % self.layer)
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
        print("New layer is %s" % self.layer)

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
            elif event.key == pygame.K_c:
                # change the colliding state
                self.change_collide_state(True)
            elif event.key == pygame.K_v:
                # change the colliding state
                self.change_collide_state(False)
            elif event.key == pygame.K_UP:
                # change the layer
                self.change_layer(+1)
            elif event.key == pygame.K_DOWN:
                # change the layer
                self.change_layer(-1)
            elif event.key == pygame.K_h:
                # display or not the toolbox
                self.display_tb = not self.display_tb
            elif event.key == pygame.K_o:
                # adding spawn
                xp, yp = pygame.mouse.get_pos()
                rpos = xp // REAL_TS + yp // REAL_TS * self.tmap["width"]
                lay = self.layer if xp <= self.tmap["width"] * REAL_TS else Editor.get_next_layer(self.layer)
                if lay != self.layer:
                    rpos = (xp - self.tmap["width"] * REAL_TS) // REAL_TS + yp // REAL_TS * self.tmap["width"]
                self.entry.reset()
                self.entry.set_placeholder("frommap(int)")
                frommap = self.entry.get_text()
                try:
                    frommap = int(frommap)
                except ValueError:
                    print("Need an integer")
                else:
                    self.tmap["spawns"].append({"oncase": rpos, "frommap": frommap})
            elif event.key == pygame.K_p:
                # adding tp
                xp, yp = pygame.mouse.get_pos()
                rpos = xp // REAL_TS + yp // REAL_TS * self.tmap["width"]
                lay = self.layer if xp <= self.tmap["width"] * REAL_TS else Editor.get_next_layer(self.layer)
                if lay != self.layer:
                    rpos = (xp - self.tmap["width"] * REAL_TS) // REAL_TS + yp // REAL_TS * self.tmap["width"]
                self.entry.reset()
                self.entry.set_placeholder("tomap(int)")
                tomap = self.entry.get_text()
                try:
                    tomap = int(tomap)
                except ValueError:
                    print("Need an integer")
                else:
                    self.tmap["tp"].append({"oncase": rpos, "tomap": tomap})
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
            lay = self.layer if xp <= self.tmap["width"] * REAL_TS else Editor.get_next_layer(self.layer)
            if lay != self.layer:
                rpos = (xp - self.tmap["width"] * REAL_TS) // REAL_TS + yp // REAL_TS * self.tmap["width"]

            if (self.display_tb and xp < W - 200) or not self.display_tb:
                if self.clic == 1:
                    # put a block
                    self.tmap[lay][rpos]["id"] = self.current_block
                elif self.clic == 2:
                    # destroy a block
                    self.tmap[lay][rpos]["id"] = None
                elif self.clic == 3:
                    # pick a chu ... pick a block
                    self.current_block = self.tmap[lay][rpos]["id"]
            elif self.display_tb and xp >= W - 200:
                # we clicked in the toolbox
                ry = yp // 20
                if 0 <= ry < len(self.tb):
                    self.tb[ry][1]() if self.tb[ry][1] else None
        elif event.type == pygame.MOUSEMOTION:
            xp, yp = event.pos
            rpos = xp // REAL_TS + yp // REAL_TS * self.tmap["width"]
            lay = self.layer if xp <= self.tmap["width"] * REAL_TS else Editor.get_next_layer(self.layer)
            if lay != self.layer:
                rpos = (xp - self.tmap["width"] * REAL_TS) // REAL_TS + yp // REAL_TS * self.tmap["width"]

            if rpos < 0 or rpos >= len(self.tmap[lay]):
                self.clic = 0

            if (self.display_tb and xp < W - 200) or not self.display_tb:  # no drag and drop in the toolbox
                if self.clic == 1:
                    # put a block
                    self.tmap[lay][rpos]["id"] = self.current_block
                elif self.clic == 2:
                    # destroy a block
                    self.tmap[lay][rpos]["id"] = None
                elif self.clic == 3:
                    # pick a chu ... pick a block
                    self.current_block = self.tmap[lay][rpos]["id"]
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