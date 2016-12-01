# maps editor (using tileset and not separate tiles in the assets/tiles folder)
# made by Loodoor on the 27th of August, 2016
# Revision on the 1st of December, 2016
# All rights reserved

from arg_parse import parse_args


def main():
    temp = parse_args()

    import core, pygame, functions

    pygame.init()
    functions.start()

    editor = core.Editor()

    editor.run(temp)

    print("Quitting")
    pygame.quit()