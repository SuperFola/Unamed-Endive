# image loader
import Unamed
import os

print(os.getcwd())
print(os.path.exists("assets/gui/bulle.png"))

print("loading assets from python")
Unamed.loadImage("assets/gui/bulle.png", "bulle")