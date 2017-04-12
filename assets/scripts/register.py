# author : Loodoor
# all rights reserved

# PLEASE DO NOT DELETE THIS FILE
import Unamed
from Unamed import upr
import ast, socket, hashlib

###############################################################################
# game stuff                                                                  #
###############################################################################

include = lambda n: __import__("assets/scripts/" + n)
sha256 = lambda w: hashlib.sha256(w.encode()).hexdigest()
playername = Unamed.getPlayerName()

# CONSTANTS
PRGS_SAVING_PATH = "saves/" + playername + "/progess"
SWITCHS_SAVING_PATH = "saves/" + playername + "/switchs"
VARS_SAVING_PATH = "saves/" + playername + "/vars"
keys_ev_code = ast.literal_eval(open("assets/scripts/keysevents.json").read())
PNJkind = {"normal": 0, "special": 1, "system": 2}
BUFFER = 2 ** 10
HOST, PORT = "", 0

# a table to track down the progress of the player in the adventure
# useful if you want to create an event at a specific moment of the game
#     see events-defining.md (at root top) to know how it is working
_progress = {}
_switchs = {}  # TODO : create switchs or load them from file
_vars = {}     # TODO : same

# socket for the network
sock = None

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

# function to init the socket
def netconnect(h, p, proto="TCP"):
    global sock, HOST, PORT
    HOST, PORT = h, p
    if proto == "UDP":
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    elif proto == "TCP":
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))

# handle the errors about the network
def nethandle_error(func, *args):
    try: return func(*args)
    except socket.gaierror as sge: print("PY>", sge)
    except ConnectionResetError as cre: print("PY> Socket not opened")

# function to send messages through network
def netsend(message, proto="TCP"):
    global sock, HOST, PORT
    send = lambda m, c: sock.sendto(m, c) if proto == "UDP" else sock.sendall(m)
    nethandle_error(send, str(message).encode(), (HOST, PORT))

# function to receive raw strings from network
def netrecv():
    global sock, BUFFER
    recv = lambda: sock.recv(BUFFER).decode()
    r = nethandle_error(recv)
    return str(r)

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

print("PY> cwd", os.getcwd())
print("PY> PYTHONHOME", os.environ.get("PYTHONHOME"))
print("PY> HOME", os.environ.get("HOME"))

