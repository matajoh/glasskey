#ifndef _GK_FRAME_H_
#define _GK_FRAME_H_

#include <cstdint>
#include <thread>

namespace gk
{
class Frame {
public:
    Frame(std::uint16_t rows, std::uint16_t cols, std::uint16_t resolution, float frame_rate);

    std::vector<char> &buffer();

    void set(std::uint16_t row, std::uint16_t col, char value);
    char get(std::uint16_t row, std::uint16_t col) const;

    void start();

    void stop();

private:
    std::thread m_main_thread;
    std::vector<char> m_current;
    std::uint16_t m_rows;
    std::uint16_t m_cols;
};
}

#endif