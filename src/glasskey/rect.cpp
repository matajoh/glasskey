#include "glasskey/glasskey.h"

#include <algorithm>

namespace gk
{

Rect::Rect(Index left, Index top, Size width, Size height) :
    left(left),
    top(top),
    width(width),
    height(height),
    right(left + width),
    bottom(top + height)
    {}

Rect Rect::translate(Index dx, Index dy) const
{
    return Rect(left + dx, top + dy, width, height);
}

Rect Rect::move(Index left, Index top) const
{
    return Rect(left, top, width, height);
}

Rect Rect::clip(Size width, Size height) const
{
    Index left = fix_range(this->left, width);
    Index top = fix_range(this->top, height);
    Index right = fix_range(this->right, width);
    Index bottom = fix_range(this->bottom, height);
    return Rect(left, top, right-left, bottom-top);
}

std::uint32_t Rect::area() const
{
    return static_cast<std::uint32_t>(width) * static_cast<std::uint32_t>(height);
}
}