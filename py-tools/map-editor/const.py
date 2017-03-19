TS = 17
REAL_TS = 16
DEF_WIDTH = 16
DEF_HEIGHT = 8
VERSION = "v0.3.0"
TITLE = "Unamed Maps Editor (Py3 version)"
W, H = 1040, 640
WSIZE = W, H
controls = [
    "s : save the map",
    "l : load a map",
    "c : the block under the cursor is now colliding",
    "v : the block under the cursor is no longer colliding",
    "h : to show or not the toolbox",
    "p : add a tp to the pointed block",
    "left clic : put a block",
    "middle clic : set/unset collision",
    "right clic : put a tp",
    "up/right : change the layer",
    "down_left : change the layer"
]
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREY = (128, 128, 128)
BLUE = (0, 0, 255)
GREEN = (0, 255, 0)
PURPLE = (255, 0, 255)
YELLOW = (0, 255, 255)
BROWN = (255, 255, 0)

COLORS = [
    WHITE,
    GREY,
    RED,
    BLUE,
    GREEN,
    PURPLE,
    YELLOW,
    BROWN
]