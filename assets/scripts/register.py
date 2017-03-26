# author : Loodoor
# all rights reserved

# PLEASE DO NOT DELETE THIS FILE
import Unamed
from Unamed import upr
import ast

###############################################################################
# game stuff                                                                  #
###############################################################################

include = lambda n: __import__("assets/scripts/" + n)
playername = Unamed.getPlayerName()
PRGS_SAVING_PATH = "saves/" + playername + "/progess"
SWITCHS_SAVING_PATH = "saves/" + playername + "/switchs"
VARS_SAVING_PATH = "saves/" + playername + "/vars"
keys_ev_code = ast.literal_eval(open("assets/scripts/keysevents.json").read())

# a table to track down the progress of the player in the adventure
# useful if you want to create an event at a specific moment of the game
#     see events-defining.md (at root top) to know how it is working
_progress = {}
_switchs = {}  # TODO : create switchs or load them from file
_vars = {}     # TODO : same

# load progress/switchs...
def load_stuff():
    global progress
    if os.path.exists(PRGS_SAVING_PATH):
        with open(PRGS_SAVING_PATH) as file:
            _progress = ast.literal_eval(file.read())
    if os.path.exists(SWITCHS_SAVING_PATH):
        with open(SWITCHS_SAVING_PATH) as file:
            _switchs = ast.literal_eval(file.read())
    if os.path.exists(VARS_SAVING_PATH):
        with open(VARS_SAVING_PATH) as file:
            _vars = ast.literal_eval(file.read())
# save progress/switchs...
def save_stuff():
    with open(PRGS_SAVING_PATH, "w") as file:
        file.write(str(_progress))
    with open(SWITCHS_SAVING_PATH, "w") as file:
        file.write(str(_switchs))
    with open(VARS_SAVING_PATH, "w") as file:
        file.write(str(_vars))

# scripts only for the game do not modify/delete them
Unamed.registerScript("runOnceWhenClosing", "closing.py")
Unamed.registerScript("runOnceWhenStarting", "doc_extract.py")
Unamed.registerScript("runWhenUpdatingGame", "ev_checking.py")

###############################################################################
# your own scripts here                                                       #
###############################################################################

Unamed.registerScript("runOnceWhenStarting", "img_loader.py")
Unamed.registerScript("runWhenProcessingEvents", "script.py")

###############################################################################
# functions that need to be execute after everything has been loaded          #
###############################################################################

load_stuff()

print("cwd", os.getcwd())
print("PYTHONHOME", os.environ.get("PYTHONHOME"))
print("HOME", os.environ.get("HOME"))

