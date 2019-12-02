""" Example demonstrating the use of multiple windows at once """

using PyCall

gk = pyimport("glasskey")

const ROWS = 18
const COLS = 30

function main()
    left = gk.create_grid(ROWS, COLS, "Left")
    right = gk.create_grid(ROWS, COLS, "Right")
    gk.start()

    bounds = gk.Rect(0, 3, 3, 3)
    left.draw(9, COLS-5, "multiitlum")
    right.draw(9, -5, "multiitlum")
    delta_x = 1
    for _ in 1:1000
        left.clear(bounds)
        right.clear(bounds.translate(-COLS, 0))

        bounds = bounds.translate(delta_x, 0)

        if bounds.right == 2*COLS || bounds.left == 0
            delta_x *= -1
        end

        left.draw(bounds, "+")
        right.draw(bounds.translate(-COLS, 0), "+")

        left.blit();
        right.blit();

        gk.next_frame()
    end

    gk.stop()
end

main()
