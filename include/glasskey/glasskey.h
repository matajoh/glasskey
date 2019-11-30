#ifndef _GK_H_
#define _GK_H_

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <mutex>
#include <atomic>

namespace gk
{

struct Color
{
    /** Default constructor. */
    Color();

    /** Constructor.
     * \param red Red channel [0,1]
     * \param green Green channel [0,1]
     * \param blue Blue channel [0,1]
     */
    Color(std::float_t red, std::float_t green, std::float_t blue);

    /** Construct a color object from byte values.
     * \param red Red channel
     * \param green Green channel
     * \param blue Blue channel
     * \return a valid color object
     */
    static Color from_bytes(std::uint8_t red, std::uint8_t green, std::uint8_t blue);

    /** Equality operator.
     *  \param other the color to compare with
     *  \return whether the colors have the same values
     */
    bool operator==(const Color &other) const;

    /** Inequality operator.
     *  \param other the color to compare with
     *  \return whether the colors have different values
     */
    bool operator!=(const Color &other) const;

    /** The red value [0,1] */
    std::float_t r;
    /** The green value [0,1] */
    std::float_t g;
    /** The blue value [0,1] */
    std::float_t b;

    friend std::ostream &operator<<(std::ostream &os, const Color &grid);
    std::string to_string() const;
};

namespace Colors
{
const Color Black = Color::from_bytes(0, 0, 0);
const Color White = Color::from_bytes(255, 255, 255);
const Color Red = Color::from_bytes(255, 0, 0);
const Color Maroon = Color::from_bytes(128, 0, 0);
const Color Pink = Color::from_bytes(255, 200, 220);
const Color Brown = Color::from_bytes(170, 110, 40);
const Color Orange = Color::from_bytes(255, 150, 0);
const Color Coral = Color::from_bytes(255, 215, 180);
const Color Olive = Color::from_bytes(128, 128, 0);
const Color Yellow = Color::from_bytes(255, 235, 0);
const Color Beige = Color::from_bytes(255, 250, 200);
const Color Lime = Color::from_bytes(190, 255, 0);
const Color Green = Color::from_bytes(0, 190, 0);
const Color Mint = Color::from_bytes(170, 255, 195);
const Color Teal = Color::from_bytes(0, 128, 128);
const Color Cyan = Color::from_bytes(100, 255, 255);
const Color Navy = Color::from_bytes(0, 0, 128);
const Color Blue = Color::from_bytes(67, 133, 255);
const Color Purple = Color::from_bytes(130, 0, 150);
const Color Lavender = Color::from_bytes(230, 190, 255);
const Color Magenta = Color::from_bytes(255, 0, 255);
const Color Gray = Color::from_bytes(128, 128, 128);
} // namespace Colors

typedef std::int16_t Index;
typedef std::uint16_t Size;

inline Index fix_range(Index value, Size max)
{
    if (value < 0)
    {
        return 0;
    }

    if (value > max)
    {
        return max;
    }

    return value;
}

struct Rect
{
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

    std::string to_string() const;
};

struct Letter
{
    Letter();
    Letter(char value, const Color &color);
    char value;
    Color color;
    std::string to_string() const;
};

class Row : public std::vector<Letter>
{
public:
    Row(Size cols, const Color &color = Colors::White);
};

class TextGrid
{
public:
    TextGrid(TextGrid &&other);

    TextGrid &map_color(char value, const Color &color);
    TextGrid &unmap_color(char value);
    TextGrid &draw(Index row, Index col, const std::string &values);
    TextGrid &draw(Index row, Index col, const std::vector<Letter> &letters);
    TextGrid &draw(const Rect &rect, char value);
    TextGrid &clear(Index row, Index col, Size cols);
    TextGrid &clear(const Rect &rect);
    const Letter &get(Index row, Index col) const;
    const Row &get(Index row) const;
    Size rows() const;
    Size cols() const;
    const std::string &title() const;
    int &id();
    int id() const;
    bool is_dirty() const;
    void draw_rows();
    std::string to_string() const;
    
    friend std::shared_ptr<TextGrid> create_grid(Size rows, Size cols, const std::string &title = "Title", const Color &default_color = Colors::White);

private:
    TextGrid(Size rows, Size cols, const std::string &title, const Color &default_color);

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

std::ostream &operator<<(std::ostream &os, const Color &grid);
std::ostream &operator<<(std::ostream &os, const Rect &grid);
std::ostream &operator<<(std::ostream &os, const Letter &grid);
std::ostream &operator<<(std::ostream &os, const TextGrid &grid);


void init(const std::vector<std::string> &args = {});

void start();

void stop();

void destroy_grid(std::shared_ptr<TextGrid> text_grid);

} // namespace gk

#endif