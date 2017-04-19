import pygame as p
import glob as g

TS = 16
NTS = 17

p.init()

e = p.display.set_mode((128, 128))
i = p.image.load("../assets/.dontupload/.tilesets/house interieur.png").convert()
n = p.Surface((
                (i.get_width() // TS) * NTS,
                (i.get_height() // TS) * NTS
))
n.fill((255,0,255))
ts = []

for h in range(i.get_height() // TS):
    for w in range(i.get_width() // TS):
        ts.append(i.subsurface(w * TS, h * TS, TS, TS))
for i, e in enumerate(ts):
    tx, ty = i % (n.get_width() // NTS), i // (n.get_width() // NTS)
    n.blit(e, (tx * NTS, ty * NTS))
p.image.save(n, "test.png")

p.quit()