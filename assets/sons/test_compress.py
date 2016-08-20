import pygame, glob, pickle

pygame.init()

e = pygame.display.set_mode((480, 300))
for file in glob.glob("*.*"):
    if '.ogg' in file:
        s = pygame.mixer.Sound(file)
        pickle.Pickler(open(file + ".datas", "wb")).dump(s.get_raw())

pygame.quit()