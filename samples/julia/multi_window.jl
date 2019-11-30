using PyCall
using Dates

gk = pyimport("glasskey")

const ROWS = 18
const COLS = 30

left = gk.create_grid(ROWS, COLS, "Left")
right = gk.create_grid(ROWS, COLS, "Right")
gk.start()

frame_time = Millisecond(30)

bounds = gk.Rect(0, 3, 3, 3)
left.draw(9, COLS-5, "multiitlum")
right.draw(9, -5, "multiitlum")
dx = 1
for _ in 1:1000
    start = now()

    left.clear(bounds)
    right.clear(bounds.translate(-COLS, 0))

    bounds = bounds.translate(dx, 0)

    if bounds.right == 2*COLS || bounds.left == 0
        dx *= -1
    end

    left.draw(bounds, "+")
    right.draw(bounds.translate(-COLS, 0), "+")

    duration = now() - start
    if duration < frame_time
        sleep(frame_time - duration)
    end
end

gk.stop()
