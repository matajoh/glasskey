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
    if(value < 0){
        return 0;
    }

    if(value > max){
        return max;
    }

    return value;
}

class Rect {
public:
    Rect(Index left, Index top, Size width, Size height);

    Rect translate(Index dx, Index dy) const;
    Rect move(Index left, Index top) const;
    Rect clip(Size width, Size height) const;

    Size width;
    Size height;
    Index top;
    Index left;
    Index right;
    Index bottom;

    std::uint32_t area() const;
};
}

#endif