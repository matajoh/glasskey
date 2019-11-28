#include <iostream>
#include <thread>
#include <chrono>

#include "glasskey.h"

int main(int argc, char *argv[])
{
    const int ROWS = 18;
    const int COLS = 30;
    auto left = gk::create_grid(ROWS, COLS, "Left");
    auto right = gk::create_grid(ROWS, COLS, "Right");
    gk::start();

    auto frame_time = std::chrono::milliseconds(30);
    gk::Rect bounds(0, 3, 3, 3);
    left->draw(9, COLS-5, "multiitlum");
    right->draw(9, -5, "multiitlum");
    gk::Index dx = 1;
    for(int i=1; i<1000; ++i){
        auto start = std::chrono::system_clock::now();

        left->clear(bounds);
        right->clear(bounds.move(-COLS, 0));

        bounds.left() += dx;
        if(bounds.right() == 2*COLS || bounds.left() == 0){
            dx *= -1;
        }

        left->draw(bounds, '+');
        right->draw(bounds.move(-COLS, 0), '+');

        auto duration = std::chrono::system_clock::now() - start;
        if(duration < frame_time)
        {
            std::this_thread::sleep_for(frame_time - duration);
        }
    }

    gk::stop();
}