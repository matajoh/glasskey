#ifndef _GK_RECT_H_
#define _GK_RECT_H_

#include <cstdint>
#include <algorithm>

namespace gk
{
typedef std::int16_t Index;
typedef std::uint16_t Size;

inline Index fix_range(Index value, Size max)
{
    return std::min(std::max(value, Index(0)), Index(max));
}

class Rect {
public:
    Rect(Index left, Index top, Size width, Size height);

    Rect move(Index dx, Index dy) const;
    Rect clip(Size width, Size height) const;

    Size width() const;
    Size &width();
    Size height() const;
    Size &height();
    Index top() const;
    Index &top();
    Index left() const;
    Index &left();
    Index right() const;
    Index bottom() const;

    std::uint32_t area() const;

private:
    Index m_left;
    Index m_top;
    Size m_width;
    Size m_height;
};
}

#endif