// ----------------------------------------------------------------------------
//
// glasskey.h -- Main header for the Glasskey library. Glasskey makes it easy
//               to write simple OpenGL-powered ASCII-based games or animations
//
// Copyright (C) 2019 Matthew Johnson
//
// For conditions of distribution and use, see copyright notice in LICENSE
//
// ----------------------------------------------------------------------------

#ifndef _GK_H_
#define _GK_H_

#include <cstdint>
#include <cmath>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace gk
{

const int COL_WIDTH = 9;
const int ROW_HEIGHT = 15;

/** Type of signed index offsets into a grid */
typedef std::int16_t Index;

/** Type of unsigned grid sizes */
typedef std::uint16_t Size;

/** Class representing an RGB color, where RGB are floating-point values from [0, 1] */
class Color
{
public:
    /** Default constructor. */
    Color();

    /** Constructor.
     * \param red Red channel [0,1]
     * \param green Green channel [0,1]
     * \param blue Blue channel [0,1]
     */
    Color(std::float_t red, std::float_t green, std::float_t blue);

    /** Construct a color object from RGB byte values.
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
    std::float_t red() const;

    /** The green value [0,1] */
    std::float_t green() const;

    /** The blue value [0,1] */
    std::float_t blue() const;

    /** Represents of the state of the object as a string */
    std::string to_string() const;

private:
    std::float_t m_red;
    std::float_t m_green;
    std::float_t m_blue;
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

/** Initializes the underlying OpenGL context. Pass any OS-specific parameters via
 *  this method. Will be called by default the first time start() is called otherwise.
 * 
 *  \param args the vector of OpenGL command line arguments
 * 
 *  \sa start
 */
void init(const std::vector<std::string> &args = {});

/** Starts the GL event loop */
void start();

/** Stops the GL event loop */
void stop();

enum class Key {
    UP,
    RIGHT,
    LEFT,
    DOWN,
    SPACE,
    ENTER
};

/** Returns whether a key is pressed */
bool is_pressed(Key key);

class TextGrid;

/** Creates a new TextGrid.
 * 
 *  \param rows the number of rows in the grid
 *  \param cols the number of columns in the grid
 *  \param title the title. This will be displayed in the title bar of the window.
 *  \param default_color the color for all letters not otherwise specified.
 *  \return a pointer to a TextGrid
 */
std::shared_ptr<TextGrid> create_grid(Size rows, Size cols, const std::string &title = "Title", const Color &default_color = Colors::White);

/** Destroys a text grid object.
 * 
 *  \param text_grid the grid to destroy
 */
void destroy_grid(std::shared_ptr<TextGrid> text_grid);

/** Blocking call that waits for the next frame of animation.
 * 
 *  \param frames_per_second the target frame rate
 */
void next_frame(float frames_per_second = 30.0f);


/** Fix the range of a value to fall within the range [min, max].
 * 
 * \param value the value to restrict. Will be unchanged if it already
 *              falls within the range
 * \param min the minimum allowed value
 * \param max the maximum allowed value
 * \return the fixed value
*/
inline Index fix_range(Index value, Size min, Size max)
{
    if (value < min)
    {
        return min;
    }

    if (value > max)
    {
        return max;
    }

    return value;
}

/** Class representing a rectangular area within the grid */
class Rect
{
public:
    /** Constructor.
     *  \param left the left-most column
     *  \param top the top-most row
     *  \param width number of columns
     *  \param height number of rows
     */
    Rect(Index left, Index top, Size width, Size height);

    /** Returns this rectangle translated by the values provided.
     * 
     *  \param dx number of columns to translate
     *  \param dy number of rows to translate
     *  \return the translated rectangle
     */
    Rect translate(Index dx, Index dy) const;

    /** Returns this rectangle moved, or teleported, to the specified
     *  left and top position.
     * 
     *  \param left the new left-most column
     *  \param top the new top-most row
     *  \return the moved rectangle
     */
    Rect move(Index left, Index top) const;

    /** Clips this rectangle to fit within an region defined by
     *  (0, 0) -> (width, height). If the rectangle falls outside
     *  of this region it will have zero area.
     *  
     * \param width the width of the clipping region
     * \param height the height of the clipping region
     *  \return the clipped rectangle
     */
    Rect clip(Size width, Size height) const;

    /** Clips this rectangle to fit within the provided region.
     * 
     *  \param region the clipping region
     *  \return the clipped rectangle
     */
    Rect clip(const Rect &region) const;

    /** The width of the rectangle in columns */
    Size width() const;

    /** The height of the rectangle in rows */
    Size height() const;

    /** The top-most row of the rectangle */
    Index top() const;

    /** The left-most column of the rectangle */
    Index left() const;

    /** The right-most column o the rectangle */
    Index right() const;

    /** The bottom-most row of the rectangle */
    Index bottom() const;

    /** The area of the rectangle. */
    std::uint32_t area() const;

    /** Represents of the state of the object as a string */
    std::string to_string() const;

private:
    Size m_width;
    Size m_height;
    Index m_top;
    Index m_left;
};

/** Class representing a displayed letter in the grid */
class Letter
{
public:
    /** Default constructor. */
    Letter();

    /** Constructor.
     *  \param value the ASCII character value
     *  \param color the color to use when displaying the character
     */
    Letter(char value, const Color &color);

    /** The ASCII character value */
    char value() const;

    /** The display color */
    const Color &color() const;

    /** Represents of the state of the object as a string */
    std::string to_string() const;

private:
    char m_value;
    Color m_color;
};

/** Class representing a row of character in the grid */
class Row : public std::vector<Letter>
{
public:
    /** Constructor.
     *
     *  \param cols the number of columns in the row
     *  \param color the default color to use (if none is specified)
     */
    Row(Size cols, const Color &color = Colors::White);
};

/** Class representing a grid of animated ASCII text */
class TextGrid
{
public:
    /** Move constructor. */
    TextGrid(TextGrid &&other);

    /** Maps an ASCII value to have a alternate default color. This will be
     *  used for that character if no other color is specified.
     * 
     * \param value the ASCII value
     * \param color the default color for this value
     */
    TextGrid &map_color(char value, const Color &color);

    /** Removes the color mapping for an ASCII value.
     * 
     *  \param value the ASCII value to unmap
     */
    TextGrid &unmap_color(char value);

    /** Draw a string of character at the specified row and column. The
     *  text will be truncated on the right or the left if needed.
     * 
     *  \param row the row to use for writing. Must fall in the range [0, height]
     *  \param col the column to start writing at. Be any value, and string
     *             will be clipped appropriately
     *  \param values the values to draw
     */
    TextGrid &draw(Index row, Index col, const std::string &values);

    /** Draw a string of letters at the specified row and column. The letters
     *  will fully define the ASCII values and color, i.e. the default mappings
     *  will not be used.
     * 
     *  \param row the row to use for writing. Must fall in the range [0, rows]
     *  \param col the column to start writing at. Can be any value, and string
     *             will be clipped appropriately
     *  \param letters the values and colors to draw
     */
    TextGrid &draw(Index row, Index col, const std::vector<Letter> &letters);

    /** Fills a rectangular area with the specified value
     * 
     *  \param rect the areaa to fill
     *  \param value the ASCII value to use when filling
     */
    TextGrid &draw(const Rect &rect, char value);

    /** Clears a region of the specified row (i.e. sets all characters to ' ')
     * 
     *  \param row the row to clear
     *  \param col the starting column. Can be any value, and string
     *             will be clipped appropriately
     *  \param cols the number of columns to clear.
     */
    TextGrid &clear(Index row, Index col, Size cols);

    /** Clears a rectangular region (i.e. sets all characters to ' ')
     * 
     * \param rect the region
     */
    TextGrid &clear(const Rect &rect);

    /** Get the ASCII value and color at the specified index.
     *  
     *  \param row the desired row
     *  \param column the desired column
     *  \return the letter at this index
     */
    const Letter &get_letter(Index row, Index col) const;

    /** Get the desired Row.
     * 
     *  \param row must be a valid row index in the range [0, rows]
     *  \return a reference to the row
     */
    const Row &get_row(Index row) const;

    /** The number of rows in the grid */
    Size rows() const;

    /** The number of columns in the grid */
    Size cols() const;

    /** The title for this grid. Will be shown in the title bar of its window. */
    const std::string &title() const;

    /** The OpenGL window ID associated with this text grid. */
    int &id();

    /** The OpenGL window ID associated with this text grid. */
    int id() const;

    /** Represents of the state of the object as a string */
    std::string to_string() const;

    /** Requests that the TextGrid be redrawn to the screen */
    void blit();

    friend std::shared_ptr<TextGrid> create_grid(Size, Size, const std::string &, const Color &);
    friend void display_grid();
    friend void refresh_grids();

protected:
    /** Constructor. Protected due to the need for the factory to manage creation
     *  and registration with the GL thread.
     */
    TextGrid(Size rows, Size cols, const std::string &title, const Color &default_color);

    /** Draws the rows of this TextGrid to the current in-context GL window. */
    void draw_rows();

    /** Whether the text grid needs to be redrawn */
    bool is_dirty();

private:
    const Color &get_color(char value) const;
    std::vector<Row> m_grid;
    std::map<char, Color> m_color_map;
    Color m_default_color;
    const Size m_rows;
    const Size m_cols;
    const std::string m_title;
    bool m_is_dirty;
    int m_id;
    std::mutex m_rows_mutex;
};

std::ostream &operator<<(std::ostream &os, const Color &grid);
std::ostream &operator<<(std::ostream &os, const Rect &grid);
std::ostream &operator<<(std::ostream &os, const Letter &grid);
std::ostream &operator<<(std::ostream &os, const TextGrid &grid);

} // namespace gk

#endif