using PyCall
using Dates

gk = pyimport("glasskey")

text_grid = gk.create_grid(20, 20, "Hello World")

gk.start()
text_grid.map_color("o", gk.Colors.Red)
text_grid.map_color("l", gk.Colors.Blue)
text_grid.map_color("!", gk.Colors.Green)

frame_time = Millisecond(100)
text = "Hello world!"
for i in 1:100
    row = i % 20
    last = (i-1) % 20
    start = now()
    text_grid.clear(last, 0, length(text))
    text_grid.draw(row, 0, text)
    duration = now() - start
    if duration < frame_time
        sleep(frame_time - duration)
    end
end

gk.stop()
