from collections import OrderedDict
import json
import sys

c = "f:/endive/assets/config/dex.json"
if sys.argv[1:]:
    if sys.argv[1:][0] == "h":
        print("""hello this is a message to help you
        h option will display this help
        r option will replace the original file (please do this only when sending in production)""")
        exit(1)
    elif sys.argv[1:][0] == "r":
        print("the original file will be replaced with the new one. a backup will be created")
        n = c
else:
    n = "f:/endive/assets/config/dex2.json"

f = json.JSONDecoder(object_pairs_hook=OrderedDict).decode(open(c).read())
open(c+".backup", "w").write(open(c).read()) if n == c else None
f["order"] = []
for k in f.keys():
    f["order"].append(k) if k != "order" else None
json.dump(f, open(n, "w"))