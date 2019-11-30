import time

import glasskey as gk

def _main():
    text_grid = gk.create_grid(20, 20, "Hello World")

    gk.start()
    text_grid.map_color('o', gk.Colors.Red)
    text_grid.map_color('l', gk.Colors.Blue)
    text_grid.map_color('!', gk.Colors.Green)

    frame_time = 100/1000
    text = "Hello world!"
    for i in range(1, 100):
        row = i % 20
        last = (i-1) % 20
        start = time.time()
        text_grid.clear(last, 0, len(text))
        text_grid.draw(row, 0, text)
        duration = time.time() - start
        if duration < frame_time:
            time.sleep(frame_time - duration)
    
    gk.stop()


if __name__ == "__main__":
    _main()
