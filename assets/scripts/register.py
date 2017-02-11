# author : Loodoor
# all rights reserved

# PLEASE DO NOT DELETE THIS FILE
import os
import Unamed
from Unamed import upr

###############################################################################
# game stuff                                                                  #
###############################################################################

include = lambda n: __import__("assets/scripts/" + n)
playername = Unamed.getPlayerName()
PRGS_SAVING_PATH = "saves/" + playername + "/progess.int"

# an integer to track down the progress of the player in the adventure
# useful if you want to create an event at a specific moment of the game
progress = 0

# load progress
def load_prgs():
    global progress
    if os.path.exists(PRGS_SAVING_PATH):
        with open(PRGS_SAVING_PATH) as file:
            progress = int(file.read())
# save progress
def save_prgs():
    with open(PRGS_SAVING_PATH, "w") as file:
        file.write(str(progress))

# scripts only for the game do not modify/delete them
Unamed.registerScript("runOnceWhenClosing", "closing.py")
Unamed.registerScript("runOnceWhenStarting", "doc_extract.py")


###############################################################################
# your own scripts here                                                       #
###############################################################################

Unamed.registerScript("runOnceWhenStarting", "img_loader.py")
Unamed.registerScript("runWhenRenderingView", "script.py")


###############################################################################
# functions that need to be execute after everything has been loaded          #
###############################################################################

load_prgs()
print("PYTHONHOME", os.environ.get("PYTHONHOME"))
print("HOME", os.environ.get("HOME"))

