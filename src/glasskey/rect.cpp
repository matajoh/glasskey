#include "glasskey/glasskey.h"

#include <algorithm>
#include <sstream>

namespace gk
{

Rect::Rect(Index left, Index top, Size width, Size height) : m_left(left),
                                                             m_top(top),
                                                             m_width(width),
                                                             m_height(height)
{
}

Rect Rect::translate(Index dx, Index dy) const
{
    return Rect(m_left + dx, m_top + dy, m_width, m_height);
}

Rect Rect::move(Index left, Index top) const
{
    return Rect(left, top, m_width, m_height);
}

Rect Rect::clip(Size width, Size height) const
{
    return this->clip({0, 0, width, height});
}

Rect Rect::clip(const Rect& region) const
{
    Index left = fix_range(this->left(), region.left(), region.right());
    Index top = fix_range(this->top(), region.top(), region.bottom());
    Index right = fix_range(this->right(), region.left(), region.right());
    Index bottom = fix_range(this->bottom(), region.top(), region.bottom());
    return Rect(left, top, right - left, bottom - top);
}

std::string Rect::to_string() const
{
    std::stringstream stream;
    stream << "Rect(left=" << m_left
           << ", top=" << m_top
           << ", width=" << m_width
           << ", height=" << m_height
           << ")";

    return stream.str();
}

Size Rect::width() const
{
    return m_width;
}

Size Rect::height() const
{
    return m_height;
}

Index Rect::left() const
{
    return m_left;
}

Index Rect::top() const
{
    return m_top;
}

Index Rect::right() const
{
    return m_left + m_width;
}

Index Rect::bottom() const
{
    return m_top + m_height;
}

std::uint32_t Rect::area() const
{
    return static_cast<std::uint32_t>(m_width) * static_cast<std::uint32_t>(m_height);
}
} // namespace gk