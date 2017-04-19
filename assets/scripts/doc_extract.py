methods = dir(Unamed)
out = [
    "Documentation of the built-in methods of the module Unamed",
    "Please do not delete the script 'register.py'. If you need to add a script to the game, just follow the documentation : https://github.com/Loodoor/Unamed-Endive/wiki/Scripting",
    "Also, if you need to import files, use the lambda include, and not import (changing the current working directory via os.chdir will also change the cwd for the game and so it won't be able to load its resources !)",
    "When you receive an event in the python scripting, it is a dict",
    "To check the code of a key event, you can do ev['key'] == keys_ev_code.index('A') (will check if it is the key A)"
]

for method in methods:
    if method[:2] != "__" and method != "error":
        out.append(method + "\n" + "-" * len(method) + "\n" + str(getattr(Unamed, method).__doc__))

with open("assets/scripts/data/scripting_doc.txt", "w") as file:
    file.write("\n\n".join(out))