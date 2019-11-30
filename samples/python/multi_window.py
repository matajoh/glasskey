import time

import glasskey as gk

ROWS = 18
COLS = 30

def _main():
    left = gk.create_grid(ROWS, COLS, "Left")
    right = gk.create_grid(ROWS, COLS, "Right")
    gk.start()

    frame_time = 30/1000

    bounds = gk.Rect(0, 3, 3, 3)
    left.draw(9, COLS-5, "multiitlum")
    right.draw(9, -5, "multiitlum")
    dx = 1
    for _ in range(1000):
        start = time.time()

        left.clear(bounds)
        right.clear(bounds.translate(-COLS, 0))

        bounds = bounds.translate(dx, 0)

        if bounds.right == 2*COLS or bounds.left == 0:
            dx *= -1

        left.draw(bounds, '+')
        right.draw(bounds.translate(-COLS, 0), '+')

        duration = time.time() - start
        if duration < frame_time:
            time.sleep(frame_time - duration)
    
    gk.stop()


if __name__ == "__main__":
    _main()
