import copy


class Historic:
    def __init__(self):
        self.content = []
        self.current = 0

    def add(self, new):
        print("in")
        self.content.append(copy.deepcopy(new))
        self.current = len(self.content) - 1

    def set(self, x):
        if x < len(self.content):
            self.current = x
            return copy.deepcopy(self.content[x])
        return copy.deepcopy(self.content[self.current])

    def empty(self):
        self.content = []
        self.current = 0