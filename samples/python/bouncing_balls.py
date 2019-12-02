""" Example showing animated balls bouncing off the edges of the grid """

import glasskey as gk

class Ball:
    def __init__(self, bounds: gk.Rect, dx: float, dy: float, value: str):
        self.x = float(bounds.left)
        self.y = float(bounds.top)
        self.dx = dx
        self.dy = dy
        self.bounds = bounds
        self.value = value

    def update(self, text_grid: gk.TextGrid):
        self.x += self.dx
        self.y += self.dy
        left = int(self.x)
        top = int(self.y)
        right = left + self.bounds.width
        bottom = top + self.bounds.height
        if left < 0:
            self.x = 0
            self.dx *= -1
        if right >= text_grid.cols:
            self.x = text_grid.cols - self.bounds.width
            self.dx *= -1
        if top < 0:
            self.y = 0
            self.dy *= -1
        if bottom >= text_grid.rows:
            self.y = text_grid.rows - self.bounds.height
            self.dy *= -1

    def draw(self, text_grid: gk.TextGrid):
        text_grid.clear(self.bounds)
        self.bounds = self.bounds.move(int(self.x), int(self.y))
        text_grid.draw(self.bounds, self.value)


def _main():
    text_grid = gk.create_grid(45, 75, "Bouncing Balls")

    red = Ball(gk.Rect(1, 1, 3, 3), 0.2, 0.4, '*')
    green = Ball(gk.Rect(10, 12, 5, 5), -0.2, 0.1, '=')
    blue = Ball(gk.Rect(100, 40, 2, 2), 0.8, -0.4, 'o')

    text_grid.map_color('*', gk.Colors.Red)
    text_grid.map_color('=', gk.Colors.Green)
    text_grid.map_color('o', gk.Colors.Blue)

    gk.start()

    for _ in range(300):
        red.update(text_grid)
        green.update(text_grid)
        blue.update(text_grid)

        red.draw(text_grid)
        green.draw(text_grid)
        blue.draw(text_grid)

        text_grid.blit()

        gk.next_frame()

    gk.stop()


if __name__ == "__main__":
    _main()
