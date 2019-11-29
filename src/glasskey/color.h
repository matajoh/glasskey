#ifndef _GK_COLOR_H_
#define _GK_COLOR_H_

#include <cstdint>
#include <cmath>

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

} // namespace gk

#endif