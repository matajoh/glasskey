using PyCall
using Dates

gk = pyimport("glasskey")

mutable struct Ball
    x::Float64
    y::Float64
    dx::Float64
    dy::Float64
    bounds
    value::String
end

function update!(ball::Ball, text_grid)
    ball.x += ball.dx
    ball.y += ball.dy
    left = floor(Int64, ball.x)
    top = floor(Int64, ball.y)
    right = left + ball.bounds.width
    bottom = top + ball.bounds.height
    if left < 0
        ball.x = 0
        ball.dx *= -1
    end

    if right >= text_grid.cols
        ball.x = text_grid.cols - ball.bounds.width
        ball.dx *= -1
    end

    if top < 0
        ball.y = 0
        ball.dy *= -1
    end

    if bottom >= text_grid.rows
        ball.y = text_grid.rows - ball.bounds.height
        ball.dy *= -1
    end
end

function draw!(ball::Ball, text_grid)
    text_grid.clear(ball.bounds)
    ball.bounds = ball.bounds.move(floor(Int64, ball.x), floor(Int64, ball.y))
    text_grid.draw(ball.bounds, ball.value)
end

function main()
    text_grid = gk.create_grid(45, 75, "Bouncing Balls")

    red = Ball(1, 1, 0.2, 0.4, gk.Rect(1, 1, 3, 3), "+")
    green = Ball(10, 12, -0.2, 0.1, gk.Rect(10, 12, 5, 5), "=")
    blue = Ball(100, 40, 0.8, -0.4, gk.Rect(100, 40, 2, 2), "o")

    text_grid.map_color("+", gk.Colors.Red)
    text_grid.map_color("=", gk.Colors.Green)
    text_grid.map_color("o", gk.Colors.Blue)

    gk.start()

    frame_time = Millisecond(30)
    for _ in 1:1000
        start = now()
        update!(red, text_grid)
        update!(green, text_grid)
        update!(blue, text_grid)

        draw!(red, text_grid)
        draw!(green, text_grid)
        draw!(blue, text_grid)

        duration = now() - start
        if duration < frame_time
            sleep(frame_time - duration)
        end
    end

    gk.stop()
end

main()
