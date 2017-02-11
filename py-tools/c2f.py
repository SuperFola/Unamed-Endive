import pygame
import sys
import os


def main():
    # start
    pygame.init()
    ec = pygame.display.set_mode((128, 148))
    pygame.display.set_caption("Calque2folder")
    files = sys.argv[1:]

    # loop
    for file in [os.path.join("..", "assets", ".calques_pnj", f) for f in files]:
        # loading
        im = pygame.image.load(file).convert_alpha()

        # rendering
        pygame.draw.rect(ec, (0, 0, 0), (0, 0) + ec.get_size())
        ec.blit(im, (0, 0))

        # cutting
        ims = [pygame.Surface((128 // 4, 148 // 4)) for _ in range(16)]
        for u in range(16):
            ims[u].fill((255, 0, 255))
            ims[u].blit(im, (0, 0), ((u % 4) * 128 // 4, (u // 4) * 148 // 4, 128 // 4, 148 // 4))

        # saving
        # directory = os.path.join("..", "assets", "pnj", ''.join(random.sample(list(string.ascii_lowercase), 5)))
        directory = os.path.join("..", "assets", "pnj", os.path.basename(file).split('.')[0])
        try:
            os.mkdir(directory)
        except FileExistsError:
            pass
        for c, new in enumerate(ims):
            new.set_colorkey(new.get_at((0, 0)))
            if 0 <= c < 4:
                pref = "down"
            elif 4 <= c < 8:
                pref = "left"
            elif 8 <= c < 12:
                pref = "right"
            else:
                pref = "up"
            pygame.image.save(new, os.path.join(directory, pref + str(c % 4) + '.png'))

        # blitting
        pygame.display.flip()

        # waiting
        done = False
        while not done:
            for event in pygame.event.get():
                if event.type == pygame.KEYUP:
                    done = True

    # quit
    pygame.quit()


if __name__ == '__main__':
    main()