""" Hello World sample """

import glasskey as gk

ROWS = 2*gk.ColumnWidth
COLS = 2*gk.RowHeight
BALL_SIZE = 3


def _main():
    # create a TextGrid
    text_grid = gk.create_grid(ROWS, COLS, "Hello World")

    # map certain ASCII characters to a color
    text_grid.map_color('o', gk.Colors.Red)
    text_grid.map_color('l', gk.Colors.Blue)
    text_grid.map_color('!', gk.Colors.Green)

    # start the GL main loop
    gk.start()

    # the animation loop
    bounds = gk.Rect(COLS-BALL_SIZE, ROWS, BALL_SIZE, BALL_SIZE)
    row = 0
    text = "Hello world!"
    for _ in range(1, 100):
        # clear the grid
        text_grid.clear(row, 0, len(text))
        text_grid.clear(bounds)

        # animate
        bounds = bounds.translate(0, -1)
        if bounds.bottom == 0:
            bounds = bounds.translate(0, ROWS+BALL_SIZE)

        row = (row + 1) % ROWS

        # we can draw text directly to the grid
        text_grid.draw(row, 0, text)

        # we can draw rectangles of the same character
        text_grid.draw(bounds, 'x')

        # both of the above methods use the grid's default
        # color mapping. We can control the coloring directly
        # as shown below
        letters = [
            gk.Letter('l', gk.Colors.Cyan),
            gk.Letter('o', gk.Colors.Magenta),
            gk.Letter('l', gk.Colors.Yellow),
            gk.Letter('!', gk.Colors.White)
        ]
        text_grid.draw(ROWS//2, COLS//2, letters)

        # key input works via a polling model
        if gk.is_pressed(gk.Key.Left):
            text_grid.draw(0, 0, 'L')
        else:
            text_grid.draw(0, 0, ' ')

        # we let GL know that the TextGrid is ready to be redrawn
        text_grid.blit()

        # Now we wait until it is time for the next frame.
        # You can optionally pass a target framerate
        # (default is 30hz)
        gk.next_frame(10)

    # stop the GL main loop
    gk.stop()


if __name__ == "__main__":
    _main()
