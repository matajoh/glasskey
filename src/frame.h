#ifndef _GK_FRAME_H_
#define _GK_FRAME_H_

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <map>

#include "color.h"

namespace gk
{

typedef std::uint16_t Index;

struct Letter {
    Letter();
    Letter(char value, const Color& color);
    char value;
    Color color;
};

class Row : public std::vector<Letter> {
public:
    Row(Index cols, const Color& color = Colors::White);
};

struct Rect {
    Rect(Index left, Index top, Index width, Index height) :
        left(left),
        top(top),
        right(left + width),
        bottom(top + height)
    {}
    Index top;
    Index left;
    Index right;
    Index bottom;
};

class Frame {
public:
    Frame(Index rows, Index cols, const std::string &title="Title", const Color &default_color = Colors::White);
    ~Frame();

    static Frame &Instance();
    static Frame &Init(Index rows, Index cols, const std::string &title="Title");

    Frame& map_color(char value, const Color &color);
    Frame& unmap_color(char value);
    Frame& set(Index row, Index col, const std::string &values);
    Frame& set(Index row, Index col, const std::vector<Letter> &letters);
    Frame& set(const Rect& rect, char value);
    Frame& clear(Index row, Index col, Index cols);
    Frame& clear(const Rect& rect);
    const Letter &get(Index row, Index col) const;
    const Row &get(Index row) const;

    void start();
    void stop();
    void refresh();

private:
    static void resize(int width, int height);
    static void display();
    static void main_loop();
    static void close();
    static std::unique_ptr<Frame> m_instance;

    void draw_rows();
    const Color &get_color(char value) const;
    std::thread m_main_thread;
    std::vector<Row> m_grid;
    std::map<char, Color> m_color_map;
    Color m_default_color;
    const Index m_rows;
    const Index m_cols;
    const std::string m_title;
    int m_width;
    int m_height;
    std::mutex m_rows_mutex;
    std::atomic_bool m_running;
    bool m_init;
};
}

#endif