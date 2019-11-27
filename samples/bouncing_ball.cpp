#include <thread>
#include <chrono>

#include "text_grid.h"

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

    void update(const gk::TextGrid& text_grid)
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
        if(right >= text_grid.cols()){
            m_x = float(text_grid.cols() - m_bounds.width());
            m_dx *= -1;
        }
        if(top < 0){
            m_y = 0;
            m_dy *= -1;
        }
        if(bottom >= text_grid.rows()){
            m_y = float(text_grid.rows() - m_bounds.height());
            m_dy *= -1;
        }
    }

    void draw(gk::TextGrid &text_grid)
    {
        text_grid.clear(m_bounds);
        m_bounds.left() = gk::Index(m_x);
        m_bounds.top() = gk::Index(m_y);
        text_grid.set(m_bounds, m_value);
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
    auto &text_grid = gk::TextGrid::Init(45, 75, "Test");

    Ball red({1, 1, 3, 3}, 0.2f, 0.4f, '+');
    Ball green({10, 12, 5, 5}, -0.2f, 0.1f, '=');
    Ball blue({100, 40, 2, 2}, 0.8f, -0.4f, 'o');

    text_grid.map_color('+', gk::Colors::Red);
    text_grid.map_color('=', gk::Colors::Green);
    text_grid.map_color('o', gk::Colors::Blue);

    text_grid.start();
    auto frame_time = std::chrono::milliseconds(30);
    for(int i=0; i<1000; ++i){
        auto start = std::chrono::system_clock::now();
        red.update(text_grid);
        green.update(text_grid);
        blue.update(text_grid);

        red.draw(text_grid);
        green.draw(text_grid);
        blue.draw(text_grid);

        text_grid.refresh();
        auto dur = std::chrono::system_clock::now() - start;
        if(dur < frame_time){
            std::this_thread::sleep_for(frame_time - dur);
        }
    }

    text_grid.stop();
}

