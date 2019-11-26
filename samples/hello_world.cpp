#include <iostream>
#include <thread>
#include <chrono>

#include "frame.h"

int main(int argc, char *argv[])
{
    auto &frame = gk::Frame::Init(20, 20, "Test");
    frame.start();
    frame.map_color('o', gk::Colors::Red);
    frame.map_color('l', gk::Colors::Blue);
    frame.map_color('!', gk::Colors::Green);

    auto frame_time = std::chrono::milliseconds(100);
    std::string text = "Hello world!";
    for(int i=1; i<100; ++i){
        int row = i % 20;
        int last = (i-1) % 20;
        auto start = std::chrono::system_clock::now();
        frame.clear(last, 0, static_cast<gk::Index>(text.size()));
        frame.set(row, 0, text);
        frame.refresh();
        auto duration = std::chrono::system_clock::now() - start;
        if(duration < frame_time)
        {
            std::this_thread::sleep_for(frame_time - duration);
        }
    }

    frame.stop();
}