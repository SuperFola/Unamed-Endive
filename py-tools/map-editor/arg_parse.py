import sys
from const import *


def parse_args():
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
                    "content": eval(open(map_path_to_load).read().replace('true', 'True').replace('false', 'False').replace('null', 'None'))
                }
            except IndexError:
                print("Missing argument for option -p")
        if "-W" in sys.argv[1:]:
            try:
                width = sys.argv[1:][sys.argv[1:].index("-W") + 1]
                if not temp:
                    temp = {"type": "new map", "width": int(width)}
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
                    temp = {"type": "new map", "height": int(height)}
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
    else:
        print("No argument detected")

    return temp