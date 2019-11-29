#include <cassert>
#include <limits>

#include "color.h"

namespace gk
{
Color::Color() : Color(0, 0, 0) {}
Color::Color(std::float_t red, std::float_t green, std::float_t blue) : r(red), g(green), b(blue)
{
    assert((red >= 0.0 && red <= 1.0) || red == -std::numeric_limits<float>::infinity());
    assert((green >= 0.0 && green <= 1.0) || green == -std::numeric_limits<float>::infinity());
    assert((blue >= 0.0 && blue <= 1.0) || blue == -std::numeric_limits<float>::infinity());
}

Color Color::from_bytes(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
{
    return Color(red / 255.0f, green / 255.0f, blue / 255.0f);
}

bool Color::operator!=(const Color &other) const
{
    return this->r != other.r || this->g != other.g || this->b != other.b;
}

bool Color::operator==(const Color &other) const
{
    return !(*this != other);
}
} // namespace gk