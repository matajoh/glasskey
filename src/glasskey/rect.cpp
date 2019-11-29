#include "rect.h"

#include <algorithm>

namespace gk
{

Rect::Rect(Index left, Index top, Size width, Size height) :
    m_left(left),
    m_top(top),
    m_width(width),
    m_height(height)
    {}

Index &Rect::left()
{
    return m_left;
}

Index Rect::left() const
{
    return m_left;
}

Index &Rect::top()
{
    return m_top;
}

Index Rect::top() const
{
    return m_top;
}

Size &Rect::width()
{
    return m_width;
}

Size Rect::width() const
{
    return m_width;
}

Size &Rect::height()
{
    return m_height;
}

Size Rect::height() const
{
    return m_height;
}

Index Rect::right() const
{
    return m_left + m_width;
}

Index Rect::bottom() const
{
    return m_top + m_height;
}

Rect Rect::move(Index dx, Index dy) const
{
    return Rect(m_left + dx, m_top + dy, m_width, m_height);
}

Rect Rect::clip(Size width, Size height) const
{
    Index left = fix_range(m_left, width);
    Index top = fix_range(m_top, height);
    Index right = fix_range(this->right(), width);
    Index bottom = fix_range(this->bottom(), height);
    return Rect(left, top, right-left, bottom-top);
}

std::uint32_t Rect::area() const
{
    return static_cast<std::uint32_t>(m_width) * static_cast<std::uint32_t>(m_height);
}
}