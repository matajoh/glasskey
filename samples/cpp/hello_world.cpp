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

    // let's look at an animation loop
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
        // You can optionally pass a target framerate
        // (default is 30hz)
        gk::next_frame(10);
    }

    // stop the GL main loop
    gk::stop();
}