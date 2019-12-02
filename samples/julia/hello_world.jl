""" Hello world sample """

using PyCall

gk = pyimport("glasskey")

const ROWS = 2*gk.ColumnWidth
const COLS = 2*gk.RowHeight
const BALL_SIZE = 3

function main()

    # create a TextGrid
    text_grid = gk.create_grid(ROWS, COLS, "Hello World")

    # map certain ASCII characters to a color
    text_grid.map_color("o", gk.Colors.Red)
    text_grid.map_color("l", gk.Colors.Blue)
    text_grid.map_color("!", gk.Colors.Green)

    # start the GL main loop
    gk.start()

    # the animation loop
    bounds = gk.Rect(COLS-BALL_SIZE, ROWS, BALL_SIZE, BALL_SIZE)
    row = 0
    text = "Hello world!"
    for i in 1:100
        # clear the grid
        text_grid.clear(row, 0, length(text))
        text_grid.clear(bounds)

        # animate
        bounds = bounds.translate(0, -1)
        if bounds.bottom == 0
            bounds = bounds.translate(0, ROWS+BALL_SIZE)
        end

        row = (row + 1) % 20

        # we can draw text directly to the grid
        text_grid.draw(row, 0, text)

        # we can draw rectangles of the same character
        text_grid.draw(bounds, "x")

        # both of the above methods use the grid's default
        # color mapping. We can control the coloring directly
        # as shown below
        letters = [
            gk.Letter("l", gk.Colors.Cyan),
            gk.Letter("o", gk.Colors.Magenta),
            gk.Letter("l", gk.Colors.Yellow),
            gk.Letter("!", gk.Colors.White)
        ]
        text_grid.draw(div(ROWS, 2), div(COLS, 2), letters)

        # we let GL know that the TextGrid is ready to be redrawn
        text_grid.blit();

        # Now we wait until it is time for the next frame.
        # You can optionally pass a target framerate
        # (default is 30hz)
        gk.next_frame(10)
    end

    # stop the GL main loop
    gk.stop()
end

main()
