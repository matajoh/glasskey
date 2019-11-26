#ifndef _GK_FRAME_H_
#define _GK_FRAME_H_

#include <cstdint>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

namespace gk
{
class Frame {
public:
    Frame(std::uint16_t rows, std::uint16_t cols, const std::string &title="Title");
    ~Frame();

    static Frame &Instance();
    static Frame &Init(std::uint16_t rows, std::uint16_t cols, const std::string &title="Title");

    Frame& set(std::uint16_t row, std::uint16_t col, const std::string &value);
    char get(std::uint16_t row, std::uint16_t col) const;
    const std::string &get(std::uint16_t row) const;

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
    std::thread m_main_thread;
    std::vector<std::string> m_grid;
    const std::uint16_t m_rows;
    const std::uint16_t m_cols;
    const std::string m_title;
    int m_width;
    int m_height;
    std::mutex m_rows_mutex;
    std::atomic_bool m_running;
    bool m_init;
};
}

#endif