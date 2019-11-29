#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "glasskey.h"

using namespace pybind11::literals;
using namespace gk;

namespace py = pybind11;

PYBIND11_MODULE(_pyglasskey, m)
{
    py::class_<Color>(m, "Color")
        .def(py::init<>())
        .def(py::init<float, float, float>(), "red"_a, "green"_a, "blue"_a)
        .def_readwrite("r", &Color::r)
        .def_readwrite("g", &Color::g)
        .def_readwrite("b", &Color::b)
        .def(py::self == py::self)
        .def(py::self != py::self);

    m.def("ColorFromBytes", &Color::from_bytes, R"gkdoc(
        Construct a color object from byte values.

        Args:
            red: Red channel
            green: Green channel
            blue: Blue channel
        
        Returns:
            a valid color object
    )gkdoc",
          "red"_a, "green"_a, "blue"_a);

    m.attr("Black") = Colors::Black;
    m.attr("White") = Colors::White;
    m.attr("Red") = Colors::Red;
    m.attr("Maroon") = Colors::Maroon;
    m.attr("Pink") = Colors::Pink;
    m.attr("Brown") = Colors::Brown;
    m.attr("Orange") = Colors::Orange;
    m.attr("Coral") = Colors::Coral;
    m.attr("Olive") = Colors::Olive;
    m.attr("Yellow") = Colors::Yellow;
    m.attr("Beige") = Colors::Beige;
    m.attr("Lime") = Colors::Lime;
    m.attr("Green") = Colors::Green;
    m.attr("Mint") = Colors::Mint;
    m.attr("Teal") = Colors::Teal;
    m.attr("Cyan") = Colors::Cyan;
    m.attr("Navy") = Colors::Navy;
    m.attr("Blue") = Colors::Blue;
    m.attr("Purple") = Colors::Purple;
    m.attr("Lavender") = Colors::Lavender;
    m.attr("Magenta") = Colors::Magenta;
    m.attr("Gray") = Colors::Gray;
   
    py::class_<Rect>(m, "Rect")
        .def(py::init<Index, Index, Size, Size>(), "left"_a, "top"_a, "width"_a, "height"_a)
        .def("move", &Rect::move, "left"_a, "top"_a)
        .def("translate", &Rect::move, "dx"_a, "dy"_a)
        .def("clip", &Rect::clip, "width"_a, "height"_a)
        .def_readwrite("left", &Rect::left)
        .def_readwrite("right", &Rect::right)
        .def_readwrite("top", &Rect::top)
        .def_readwrite("bottom", &Rect::bottom)
        .def_readwrite("width", &Rect::width)
        .def_readwrite("height", &Rect::height);
    
    py::class_<Letter>(m, "Letter")
        .def(py::init<>())
        .def(py::init<char, const Color&>(), "value"_a, "color"_a)
        .def_readwrite("value", &Letter::value)
        .def_readwrite("color", &Letter::color);

    py::class_<TextGrid, std::shared_ptr<TextGrid>>(m, "TextGrid")
        .def("map_color", &TextGrid::map_color, "value"_a, "color"_a)
        .def("unmap_color", &TextGrid::unmap_color, "value"_a)
        .def("draw", py::overload_cast<Index, Index, const std::string&>(&TextGrid::draw),
             "row"_a, "col"_a, "values"_a)
        .def("draw", py::overload_cast<Index, Index, const std::vector<Letter>&>(&TextGrid::draw),
             "row"_a, "col"_a, "letters"_a)
        .def("draw", py::overload_cast<const Rect&, char>(&TextGrid::draw),
             "rect"_a, "value"_a)
        .def("clear", py::overload_cast<Index, Index, Size>(&TextGrid::clear),
             "row"_a, "col"_a, "cols"_a)
        .def("clear", py::overload_cast<const Rect&>(&TextGrid::clear),
             "rect"_a)
        .def("get", py::overload_cast<Index, Index>(&TextGrid::get, py::const_),
             "row"_a, "col"_a)
        .def("get_row", py::overload_cast<Index>(&TextGrid::get, py::const_),
             "row"_a)
        .def_property_readonly("rows", &TextGrid::rows)
        .def_property_readonly("cols", &TextGrid::cols)
        .def_property_readonly("title", &TextGrid::title);

    m.def("init", &init, "Manually initialize the library",
          "args"_a = std::vector<std::string>());
    m.def("start", &start, "Start the event loop");
    m.def("stop", &stop, "Stop the event loop");
    m.def("create_grid", &create_grid, "rows"_a, "cols"_a, "title"_a="Title", "default_color"_a=Colors::White);
    m.def("destroy_grid", &destroy_grid, "text_grid"_a);
}