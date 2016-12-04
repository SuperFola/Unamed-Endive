# coding=utf-8

import pygame


class TextBox:
    def __init__(self, window, **kwargs):
        """
        args :
            font            (default : pygame.font.SysFont("arial", 18))
            max_length      (default : 32)
            color           (default : (255, 255, 255))
            x               (default : 0)
            y               (default : 0)
            sx              (default : 120)
            sy              (default : 35)
            bgcolor         (default : (0, 0, 0))
            cli             (default : "_")
            center          (default : False)
            placeholder     (default : "")
        """
        self.window = window
        self.input = ""
        self.running = True
        self.enter = False
        self.clignote = False
        self.mdt = 0

        self.font = kwargs.get("font", pygame.font.SysFont("arial", 18))
        self.max_length = kwargs.get("max_length", 32)
        self.color = kwargs.get("color", (255, 255, 255))
        self.pos_x = kwargs.get("x", 0)
        self.pos_y = kwargs.get("y", 0)
        self.sx = kwargs.get("sx", 120)
        self.sy = kwargs.get("sy", 35)
        self.bg_color = kwargs.get("bgcolor", (0, 0, 0))
        self.cli = self.font.render(kwargs.get("cli", "_"), True, self.color)
        self.center = kwargs.get("center", False)
        self.placeholder = self.font.render(kwargs.get("placeholder", ""), True, self.color)

    def event(self, e):
        if e.type == pygame.QUIT:
            exit(1)
        if (e.type == pygame.KEYDOWN and e.key == pygame.K_ESCAPE) or (e.type == pygame.KEYDOWN and e.key == pygame.K_RETURN):
            self.running = False
            self.enter = True
        elif e.type == pygame.KEYDOWN and e.key == pygame.K_BACKSPACE:
            self.input = self.input[:-1]
        elif len(self.input) < self.max_length and e.type == pygame.KEYDOWN:
            self.input += e.unicode

    def render(self):
        pygame.draw.rect(self.window, self.bg_color, (self.pos_x, self.pos_y, self.sx, self.sy))
        texte = self.font.render(self.input, True, self.color)
        if not self.center:
            self.window.blit(self.placeholder, (self.pos_x, self.pos_y))
            self.window.blit(texte, (self.pos_x + 2 + self.placeholder.get_width(), self.pos_y))
            if self.clignote:
                self.window.blit(self.cli, (self.pos_x + 2 * 2 + self.placeholder.get_width() + texte.get_width(),
                                            self.pos_y))
        else:
            self.window.blit(self.placeholder, (self.window.get_width() // 2 - self.placeholder.get_width(),
                                                self.window.get_height() // 2 - self.placeholder.get_height() // 2))
            self.window.blit(texte, (texte.get_width() // 2, self.window.get_height() // 2 - texte.get_height() // 2))
            if self.clignote:
                self.window.blit(self.cli, (self.pos_x + 2 * 2 + self.placeholder.get_width() + texte.get_width(),
                                            self.pos_y))
        self.mdt += 1
        if not self.mdt % 100:
            self.clignote = not self.clignote
            self.mdt = 1

    def update(self):
        for event in pygame.event.get():
            self.event(event)

    def mainloop(self):
        while self.running:
            self.update()
            self.render()
            pygame.display.flip()

    def type_enter(self):
        return self.enter

    def is_running(self):
        return self.running

    def reset(self):
        self.input = ""
        self.running = True
        self.enter = False

    def set_placeholder(self, text):
        self.placeholder = self.font.render(text, True, self.color)

    def get_text(self):
        self.mainloop()

        return self.input