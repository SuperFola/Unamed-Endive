# created to get the y size of all the creatures' images
# copyright Loodoor (alias Folaefolc sometime)
import pygame
import glob
import os


def main():
    os.chdir("F:/ENDIVE")
    
    pygame.init()
    
    e = pygame.display.set_mode((120, 120))
    
    if not os.path.exists("assets/creatures/resized"):
        os.mkdir("assets/creatures/resized")
    
    ys = []
    
    for file in glob.glob("assets/creatures/*.png"):
        img = pygame.image.load(file).convert_alpha()
        ys.append(img.get_height())
    
    pygame.quit()
    
    print(sum(ys) / len(ys), sorted(ys)[:int(len(ys) / 4)], sorted(ys)[int(len(ys) * 3 / 4):], sep="\n")


if __name__ == '__main__':
    main()