# author : Loodoor
# all rights reserved

# PLEASE DO NOT DELETE THIS FILE
import Unamed

include = lambda n: __import__("assets/scripts/" + n)

Unamed.registerScript("runOnceWhenStarting", "img_loader.py")
Unamed.registerScript("runWhenRenderingView", "script.py")
Unamed.registerScript("runOnceWhenStarting", "doc_extract.py")