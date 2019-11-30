#include <iostream>
#include <thread>
#include <chrono>

#include "glasskey/glasskey.h"

int main(int argc, char *argv[])
{
    auto text_grid = gk::create_grid(20, 20, "Hello World");
    gk::start();
    text_grid->map_color('o', gk::Colors::Red);
    text_grid->map_color('l', gk::Colors::Blue);
    text_grid->map_color('!', gk::Colors::Green);

    auto frame_time = std::chrono::milliseconds(100);
    std::string text = "Hello world!";
    for(int i=1; i<100; ++i){
        int row = i % 20;
        int last = (i-1) % 20;
        auto start = std::chrono::system_clock::now();
        text_grid->clear(last, 0, static_cast<gk::Index>(text.size()));
        text_grid->draw(row, 0, text);
        auto duration = std::chrono::system_clock::now() - start;
        if(duration < frame_time)
        {
            std::this_thread::sleep_for(frame_time - duration);
        }
    }

    gk::stop();
}