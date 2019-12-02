#include "glasskey/glasskey.h"

#include <GL/freeglut_std.h>
#include <GL/freeglut_ext.h>

#include <iostream>
#include <functional>
#include <map>
#include <sstream>

namespace
{
const int COLW = 9;
const int ROWH = 15;
} // namespace

namespace gk
{
Letter::Letter() : Letter(' ', Colors::White) {}

Letter::Letter(char value, const Color &color) : m_value(value), m_color(color) {}

char Letter::value() const
{
    return m_value;
}

const Color &Letter::color() const
{
    return m_color;
}


std::string Letter::to_string() const
{
    std::stringstream stream;
    stream << "Letter(value=" << m_value
           << ", color=" << m_color
           << ")";

    return stream.str();
}

Row::Row(Size cols, const Color &color) : std::vector<Letter>(cols)
{
    std::fill(begin(), end(), Letter(' ', color));
}

TextGrid::TextGrid(Size rows, Size cols, const std::string &title, const Color &default_color) : m_rows(rows),
                                                                                                 m_cols(cols),
                                                                                                 m_title(title),
                                                                                                 m_default_color(default_color),
                                                                                                 m_id(-1)
{
    for (auto r = 0; r < rows; ++r)
    {
        m_grid.emplace_back(cols);
    }
}

TextGrid::TextGrid(TextGrid &&other) : m_rows(other.m_rows),
                                       m_cols(other.m_cols),
                                       m_title(std::move(other.m_title)),
                                       m_default_color(other.m_default_color),
                                       m_grid(std::move(other.m_grid)),
                                       m_id(other.m_id)
{
}

std::string TextGrid::to_string() const
{
    std::stringstream stream;
    stream << "TextGrid(rows=" << m_rows
           << ", cols=" << m_cols
           << ", title=" << m_title
           << ", default_color=" << m_default_color
           << ", gl_id=" << m_id
           << ")";

    return stream.str();
}

const Letter &TextGrid::get_letter(Index row, Index col) const
{
    return m_grid[row][col];
}

const Row &TextGrid::get_row(Index row) const
{
    return m_grid[row];
}

TextGrid &TextGrid::draw(Index row, Index col, const std::string &values)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    if (row < 0)
    {
        row += m_rows;
    }

    Index left = fix_range(col, 0, m_cols);
    Index right = fix_range(col + Size(values.size()), 0, m_cols);
    auto first = values.begin() + (left - col);
    auto last = first + (right - left);
    std::vector<Letter> letters;
    std::transform(first, last, std::back_inserter(letters),
                   [this](char value) -> Letter { return Letter(value, get_color(value)); });
    std::copy(letters.begin(), letters.end(), m_grid[row].begin() + left);
    m_is_dirty = true;
    return *this;
}

TextGrid &TextGrid::draw(Index row, Index col, const std::vector<Letter> &letters)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    if (row < 0)
    {
        row += m_rows;
    }

    Index left = fix_range(col, 0, m_cols);
    Index right = fix_range(col + Size(letters.size()), 0, m_cols);
    auto first = letters.begin() + (left - col);
    auto last = first + (right - left);
    std::copy(first, last, m_grid[row].begin() + left);
    m_is_dirty = true;
    return *this;
}

TextGrid &TextGrid::draw(const Rect &rect, char value)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    Letter letter(value, get_color(value));
    Rect clip = rect.clip(m_cols, m_rows);
    if (clip.area() == 0)
    {
        return *this;
    }

    auto top = m_grid.begin() + clip.top();
    auto bottom = m_grid.begin() + clip.bottom();
    for (auto row = top; row < bottom; ++row)
    {
        auto left = row->begin() + clip.left();
        auto right = row->begin() + clip.right();
        std::fill(left, right, letter);
    }

    m_is_dirty = true;

    return *this;
}

TextGrid &TextGrid::clear(Index row, Index col, Size cols)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    // TODO create function for this
    Index left = fix_range(col, 0, m_cols);
    Index right = fix_range(col + cols, 0, m_cols);
    if (right - left == 0)
    {
        return *this;
    }

    auto first = m_grid[row].begin() + (left - col);
    auto last = first + (right - left);
    std::fill(first, last, Letter());

    m_is_dirty = true;

    return *this;
}

TextGrid &TextGrid::clear(const Rect &rect)
{
    return draw(rect, ' ');
}

TextGrid &TextGrid::map_color(char value, const Color &color)
{
    m_color_map[value] = color;
    return *this;
}

TextGrid &TextGrid::unmap_color(char value)
{
    m_color_map.erase(value);
    return *this;
}

const Color &TextGrid::get_color(char value) const
{
    if (m_color_map.count(value))
    {
        return m_color_map.at(value);
    }

    return m_default_color;
}

void TextGrid::draw_rows()
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    float y = 15;
    for (auto &row : m_grid)
    {
        float x = 0;
        for (auto &letter : row)
        {
            if (letter.value() == ' ')
            {
                x += COLW;
                continue;
            }

            glColor3f(letter.color().red(),
                      letter.color().green(),
                      letter.color().blue());
            glRasterPos2f(x, y);
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter.value());
            x += COLW;
        }

        y += ROWH;
    }

    m_is_dirty = false;
}

bool TextGrid::is_dirty()
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    return m_is_dirty;
}

Size TextGrid::cols() const
{
    return m_cols;
}

Size TextGrid::rows() const
{
    return m_rows;
}

const std::string &TextGrid::title() const
{
    return m_title;
}

int &TextGrid::id()
{
    return m_id;
}

int TextGrid::id() const
{
    return m_id;
}
} // namespace gk