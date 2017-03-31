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

# CONSTANTS
PRGS_SAVING_PATH = "saves/" + playername + "/progess"
SWITCHS_SAVING_PATH = "saves/" + playername + "/switchs"
VARS_SAVING_PATH = "saves/" + playername + "/vars"
keys_ev_code = ast.literal_eval(open("assets/scripts/keysevents.json").read())
PNJkind = {"normal": 0, "special": 1, "system": 2}

# a table to track down the progress of the player in the adventure
# useful if you want to create an event at a specific moment of the game
#     see events-defining.md (at root top) to know how it is working
_progress = {}
_switchs = {}  # TODO : create switchs or load them from file
_vars = {}     # TODO : same

# load progress/switchs...
def load_stuff():
    global _progress
    # progress
    if os.path.exists(PRGS_SAVING_PATH):
        with open(PRGS_SAVING_PATH) as file:
            _progress = ast.literal_eval(file.read())
    # switchs
    if os.path.exists(SWITCHS_SAVING_PATH):
        with open(SWITCHS_SAVING_PATH) as file:
            _switchs = ast.literal_eval(file.read())
    else:
        with open("assets/config/switchs") as file:
            _switchs = ast.literal_eval(file.read())
    # global vars
    if os.path.exists(VARS_SAVING_PATH):
        with open(VARS_SAVING_PATH) as file:
            _vars = ast.literal_eval(file.read())
    else:
        with open("assets/config/vars") as file:
            _vars = ast.literal_eval(file.read())

# save progress/switchs...
def save_stuff():
    with open(PRGS_SAVING_PATH, "w") as file:
        file.write(str(_progress))
    with open(SWITCHS_SAVING_PATH, "w") as file:
        file.write(str(_switchs))
    with open(VARS_SAVING_PATH, "w") as file:
        file.write(str(_vars))

# function to trigger an event from the C++ code
def trigger_event(mid, x, y, triggtype):
    global _progress
    print("PY> trying to trigger an event on map id", mid, ", x", x, ", y", y, ", triggtype", triggtype)
    ev_onmap = _progress.get(mid, {})
    if ev_onmap and ((x, y) in ev_onmap.keys() or triggtype == "autorun"):
        # triggering the autorun events
        if triggtype == "autorun":
            for k, v in ev_onmap.items():
                if v["trigger"] == "autorun":
                    v["triggered"] = True
        # triggering other events' type
        else:
            if ev_onmap[x, y]["trigger"] == triggtype:
                ev_onmap[x, y]["triggered"] = True

# scripts only for the game do not modify/delete them
Unamed.registerScript("runOnceWhenClosing", "closing.py")
Unamed.registerScript("runOnceWhenStarting", "doc_extract.py")
Unamed.registerScript("runOnceWhenStarting", "addpnjs.py")
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

