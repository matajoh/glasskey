/** Example demonstrating the use of multiple windows at once */

#include "glasskey/glasskey.h"

int main(int argc, char *argv[])
{
    const int ROWS = 18;
    const int COLS = 30;
    auto left = gk::create_grid(ROWS, COLS, "Left");
    auto right = gk::create_grid(ROWS, COLS, "Right");
    gk::start();

    gk::Rect bounds(0, 3, 3, 3);
    left->draw(9, COLS-5, "multiitlum");
    right->draw(9, -5, "multiitlum");
    gk::Index delta_x = 1;
    for(int i=1; i<1000; ++i){
        left->clear(bounds);
        right->clear(bounds.translate(-COLS, 0));

        bounds = bounds.translate(delta_x, 0);
        if(bounds.right() == 2*COLS || bounds.left() == 0){
            delta_x *= -1;
        }

        left->draw(bounds, '+');
        right->draw(bounds.translate(-COLS, 0), '+');

        left->blit();
        right->blit();

        gk::next_frame();
    }

    gk::stop();
}