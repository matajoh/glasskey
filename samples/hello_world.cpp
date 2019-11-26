#include <iostream>
#include <thread>
#include <chrono>

#include "frame.h"

int main(int argc, char *argv[])
{
    auto &frame = gk::Frame::Init(20, 20, "Test");
    frame.start();

    auto frame_time = std::chrono::milliseconds(1000);
    for(int i=1; i<100; ++i){
        std::cout << "frame " << i << std::endl;
        int row = i % 20;
        int last = (i-1) % 20;
        auto start = std::chrono::system_clock::now();
        frame.set(last, 0, "               ");
        frame.set(row, 0, "Matthew Johnson");
        frame.refresh();
        auto duration = std::chrono::system_clock::now() - start;
        if(duration < frame_time)
        {
            std::this_thread::sleep_for(frame_time - duration);
        }
    }

    frame.stop();
}