#include <thread>
#include <chrono>

#include "frame.h"

class Ball {
public:
    Ball(const gk::Rect &bounds, float dx, float dy, char value) :
        m_x(bounds.left()),
        m_y(bounds.top()),
        m_dx(dx),
        m_dy(dy),
        m_bounds(bounds),
        m_new_bounds(bounds),
        m_value(value)
    {
    }

    void update(const gk::Frame& frame)
    {
        m_x += m_dx;
        m_y += m_dy;
        gk::Index left = gk::Index(m_x);
        gk::Index top = gk::Index(m_y);
        gk::Index right = left + m_bounds.width();
        gk::Index bottom = top + m_bounds.height();
        gk::Index dx = 0;
        gk::Index dy = 0;
        if(left < 0){
            m_x = 0;
            m_dx *= -1;
        }
        if(right >= frame.cols()){
            m_x = float(frame.cols() - m_bounds.width());
            m_dx *= -1;
        }
        if(top < 0){
            m_y = 0;
            m_dy *= -1;
        }
        if(bottom >= frame.rows()){
            m_y = float(frame.rows() - m_bounds.height());
            m_dy *= -1;
        }
    }

    void draw(gk::Frame &frame)
    {
        frame.clear(m_bounds);
        m_bounds.left() = gk::Index(m_x);
        m_bounds.top() = gk::Index(m_y);
        frame.set(m_bounds, m_value);
    }

private:
    float m_x;
    float m_y;
    float m_dx;
    float m_dy;
    gk::Rect m_bounds;
    gk::Rect m_new_bounds;
    char m_value;
};

int main(int argc, char *argv[])
{
    auto &frame = gk::Frame::Init(45, 75, "Test");

    Ball red({1, 1, 3, 3}, 0.2f, 0.4f, '+');
    Ball green({10, 12, 5, 5}, -0.2f, 0.1f, '=');
    Ball blue({100, 40, 2, 2}, 0.8f, -0.4f, 'o');

    frame.map_color('+', gk::Colors::Red);
    frame.map_color('=', gk::Colors::Green);
    frame.map_color('o', gk::Colors::Blue);

    frame.start();
    auto frame_time = std::chrono::milliseconds(30);
    for(int i=0; i<1000; ++i){
        auto start = std::chrono::system_clock::now();
        red.update(frame);
        green.update(frame);
        blue.update(frame);

        red.draw(frame);
        green.draw(frame);
        blue.draw(frame);

        frame.refresh();
        auto dur = std::chrono::system_clock::now() - start;
        if(dur < frame_time){
            std::this_thread::sleep_for(frame_time - dur);
        }
    }

    frame.stop();
}

