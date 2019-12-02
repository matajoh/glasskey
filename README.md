# glasskey
`glasskey` is a library for displaying ASCII animations using OpenGL which I developed
to help visualize some of the solutions for [Advent of Code](http://adventofcode.com).
The idea is to make it very easy to write programs that show their results as ASCII
art, which can additionally be combined with the built-in video capture functionality
in Windows 10 (using `Win` + `G`) and other app capture methods to create videos.

## Getting Started

I provide two interfaces at time of writing, one for C++ and a Python binding for the
C++. Check in [Releases](https://github.com/matajoh/glasskey/releases) for the latest
packages and wheels. Here is a [sample program](samples/cpp/hello_world.cpp) in C++:

```c++
#include "glasskey/glasskey.h"

int main(int argc, char *argv[])
{
    const gk::Size ROWS = 20;
    const gk::Size COLS = 20;
    const gk::Size BALL_SIZE = 3;

    // create a TextGrid
    auto text_grid = gk::create_grid(ROWS, COLS, "Hello World");

    // map certain ASCII characters to a color
    text_grid->map_color('o', gk::Colors::Red);
    text_grid->map_color('l', gk::Colors::Blue);
    text_grid->map_color('!', gk::Colors::Green);

    // start the GL main loop
    gk::start();

    // some loop variables
    gk::Rect bounds(COLS-BALL_SIZE, ROWS, BALL_SIZE, BALL_SIZE);
    gk::Index row = 0;
    std::string text = "Hello World!";

    // let's look at how to do animation
    for(int i=0; i<100; ++i){
        // first we clear the grid
        text_grid->clear(row, 0, static_cast<gk::Index>(text.size()));
        text_grid->clear(bounds);

        // animate        
        bounds = bounds.translate(0, -1);
        if(bounds.bottom == 0){
            bounds = bounds.translate(0, ROWS+BALL_SIZE);
        }
        row = (row + 1) % ROWS;

        // we can draw text directly to the grid
        text_grid->draw(row, 0, text);
        // we can draw rectangles of the same character
        text_grid->draw(bounds, 'x');

        // both of the above methods use the grid's default
        // color mapping. We can also control this directly
        // as shown below.
        std::vector<gk::Letter> letters = {
            {'l', gk::Colors::Cyan},
            {'o', gk::Colors::Magenta},
            {'l', gk::Colors::Yellow},
            {'!', gk::Colors::White}
        };
        text_grid->draw(ROWS/2, COLS/2, letters);

        // finally we signal that we are done drawing.
        // you can optionally pass your target framerate
        // (default is 30hz)
        gk::next_frame(10);
    }

    // stop the GL main loop
    gk::stop();
}
```

For reference, here is the same program in Python:

