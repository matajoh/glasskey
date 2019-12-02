#include <cassert>
#include <limits>
#include <sstream>

#include "glasskey/glasskey.h"

namespace gk
{
Color::Color() : Color(0, 0, 0) {}
Color::Color(std::float_t red, std::float_t green, std::float_t blue) : m_red(red), m_green(green), m_blue(blue)
{
    assert((red >= 0.0 && red <= 1.0) || red == -std::numeric_limits<float>::infinity());
    assert((green >= 0.0 && green <= 1.0) || green == -std::numeric_limits<float>::infinity());
    assert((blue >= 0.0 && blue <= 1.0) || blue == -std::numeric_limits<float>::infinity());
}

std::string Color::to_string() const
{
    std::stringstream stream;
    stream << "Color(r=" << m_red
        << ", g=" << m_green
        << ", b=" << m_blue
        << ")";
    
    return stream.str();
}

Color Color::from_bytes(std::uint8_t red, std::uint8_t green, std::uint8_t blue)
{
    return Color(red / 255.0f, green / 255.0f, blue / 255.0f);
}

bool Color::operator!=(const Color &other) const
{
    return this->m_red != other.m_red || this->m_green != other.m_green || this->m_blue != other.m_blue;
}

bool Color::operator==(const Color &other) const
{
    return !(*this != other);
}

std::float_t Color::red() const
{
    return m_red;
}

std::float_t Color::green() const
{
    return m_green;
}

std::float_t Color::blue() const
{
    return m_blue;
}
} // namespace gk