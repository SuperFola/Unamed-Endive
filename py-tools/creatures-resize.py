# created to resize all the creatures images to the same size
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
    
    for file in glob.glob("assets/creatures/*.png"):
        img = pygame.image.load(file).convert_alpha()
        x = 512
        y = int((x / img.get_width()) * img.get_height())
        new = pygame.transform.scale(img, (x, y))
        pygame.image.save(new, "assets/creatures/resized/" + os.path.basename(file))
    
    pygame.quit()


if __name__ == '__main__':
    main()