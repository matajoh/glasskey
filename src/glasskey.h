#ifndef _GK_H_
#define _GK_H_

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <mutex>
#include <atomic>

#include "color.h"
#include "rect.h"

namespace gk
{

struct Letter {
    Letter();
    Letter(char value, const Color& color);
    char value;
    Color color;
};

class Row : public std::vector<Letter> {
public:
    Row(Size cols, const Color& color = Colors::White);
};

class TextGrid {
public:
    TextGrid(TextGrid &&other);

    TextGrid& map_color(char value, const Color &color);
    TextGrid& unmap_color(char value);
    TextGrid& draw(Index row, Index col, const std::string &values);
    TextGrid& draw(Index row, Index col, const std::vector<Letter> &letters);
    TextGrid& draw(const Rect& rect, char value);
    TextGrid& clear(Index row, Index col, Size cols);
    TextGrid& clear(const Rect& rect);
    const Letter &get(Index row, Index col) const;
    const Row &get(Index row) const;
    Size rows() const;
    Size cols() const;
    const std::string &title() const;
    int &id();
    int id() const;
    bool is_dirty() const;
    void draw_rows();

    friend std::shared_ptr<TextGrid> create_grid(Size rows, Size cols, const std::string &title="Title", const Color &default_color = Colors::White);

private:
    TextGrid(Size rows, Size cols, const std::string &title, const Color &default_color);

    static void resize(int width, int height);
    static void display();

    const Color &get_color(char value) const;
    std::vector<Row> m_grid;
    std::map<char, Color> m_color_map;
    Color m_default_color;
    const Size m_rows;
    const Size m_cols;
    const std::string m_title;
    std::atomic_bool m_is_dirty;
    int m_id;
    std::mutex m_rows_mutex;
};

void init(const std::vector<std::string> &args={});

void start();

void stop();

void delete_grid(std::shared_ptr<TextGrid> text_grid);

}

#endif