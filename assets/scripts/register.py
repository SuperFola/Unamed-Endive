# author : Loodoor
# all rights reserved

# PLEASE DO NOT DELETE THIS FILE
import Unamed

###############################################################################
# game stuff                                                                  #
###############################################################################

include = lambda n: __import__("assets/scripts/" + n)
playername = Unamed.getPlayerName()

# an integer to track down the progress of the player in the adventure
# useful if you want to create an event at a specific moment of the game
progress = 0

# load progress
def load_prgs():
    global progress
    with open("saves/" + playername + "/progess.int") as file:
        progress = int(file.read())
# save progress
def save_prgs():
    with open("saves/" + playername + "/progress.int", "w") as file:
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

