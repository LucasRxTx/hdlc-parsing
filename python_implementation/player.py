
class Player:
    __slots__ = ("x", "y")

    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def move(self, velocity: int):
        if self.x + velocity < 5:
            self.x += velocity
        elif self.x - velocity >= 0:
            self.x -= velocity
        elif self.y + velocity < 5:
            self.y += velocity
        elif self.y - velocity >= 0:
            self.y -= velocity
        else:
            print("Invalid move")
            exit(1)

        print(f"player moved({velocity}) to position({self.x}, {self.y})")
