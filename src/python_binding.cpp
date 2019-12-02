#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "glasskey/glasskey.h"

using namespace pybind11::literals;
using namespace gk;

namespace py = pybind11;

PYBIND11_MODULE(_pyglasskey, m)
{
    py::class_<Color>(m, "Color")
        .def(py::init<float, float, float>(), "red"_a = 0, "green"_a = 0, "blue"_a = 0)
        .def_property_readonly("red", &Color::red)
        .def_property_readonly("green", &Color::green)
        .def_property_readonly("blue", &Color::blue)
        .def("__repr__", &Color::to_string)
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

    py::class_<Rect>(m, "Rect", R"gkdoc(
        Class representing a rectangular area within the grid

        Args:
            left: the left-most column
            top: the top-most row
            width: the number of columns
            height: the number of rows
    )gkdoc")
        .def(py::init<Index, Index, Size, Size>(), "left"_a, "top"_a, "width"_a, "height"_a)
        .def("move", &Rect::move, R"gkdoc(
            Returns this rectangle moved, or teleported, to the specified
            left and top position.
             
            Args:
                left: the new left-most column
                top: the new top-most row
            
            Returns:
                the moved rectangle
        )gkdoc",
             "left"_a, "top"_a)
        .def("translate", &Rect::translate, R"gkdoc(
            Returns this rectangle translated by the values provided.

            Args:
                dx: number of columns to translate
                dy: number of rows to translate
            
            Returns:
                the translated rectangle
        )gkdoc",
             "dx"_a, "dy"_a)
        .def("clip", py::overload_cast<Size, Size>(&Rect::clip, py::const_), R"gkdoc(
            Clips this rectangle to fit within an region defined by
            (0, 0) -> (width, height). If the rectangle falls outside
            of this region it will have zero area.

            Args:
                width: the width of the clipping region
                height: the height of the clipping region
            
            Returns:
                the clipped rectangle
        )gkdoc",
             "width"_a, "height"_a)
        .def("clip", py::overload_cast<const Rect &>(&Rect::clip, py::const_), R"gkdoc(
            Clips this rectangle to fit within the provided region.

            Args:
                region: the clipping region
            
            Returns:
                the clipped rectangle
        )gkdoc",
             "region"_a)
        .def_property_readonly("left", &Rect::left, "The left-most column")
        .def_property_readonly("right", &Rect::right, "The right-most column")
        .def_property_readonly("top", &Rect::top, "The top-most row")
        .def_property_readonly("bottom", &Rect::bottom, "The bottom-most row")
        .def_property_readonly("width", &Rect::width, "The width in columns")
        .def_property_readonly("height", &Rect::height, "The height in rows")
        .def("__repr__", &Rect::to_string);

    py::class_<Letter>(m, "Letter", R"gkdoc(
        Class representing a colored ASCII value in a grid.

        Keyword Args:
            value: the ASCII value [' ']
            color: the color [White]
    )gkdoc")
        .def(py::init<char, const Color &>(), "value"_a = ' ', "color"_a = Colors::White)
        .def("__repr__", &Letter::to_string)
        .def_readwrite("value", &Letter::value, "The ASCII character value")
        .def_readwrite("color", &Letter::color, "The display color");

    py::class_<TextGrid, std::shared_ptr<TextGrid>>(m, "TextGrid", R"gkdoc(
        Class representing a grid of animated ASCII text
    )gkdoc")
        .def("map_color", &TextGrid::map_color, R"gkdoc(
            Maps an ASCII value to have a alternate default color. This will be
            used for that character if no other color is specified.

            Args:
                value: the ASCII value
                color: the default color for this value
        )gkdoc",
             "value"_a, "color"_a)
        .def("unmap_color", &TextGrid::unmap_color, R"gkdoc(
            Removes the color mapping for an ASCII value. 

            Args:
                value: the ASCII value to unmap
            )gkdoc",
             "value"_a)
        .def("draw", py::overload_cast<Index, Index, const std::string &>(&TextGrid::draw), R"gkdoc(
            Draw a string of character at the specified row and column. The
            text will be truncated on the right or the left if needed.

            Args:
                row: the row to use for writing. Must fall in the range [0, height]
                col: the column to start writing at. Be any value, and string
                     will be clipped appropriately
                values: the values to draw
        )gkdoc",
             "row"_a, "col"_a, "values"_a)
        .def("draw",
             py::overload_cast<Index, Index, const std::vector<Letter> &>(&TextGrid::draw), R"gkdoc(
            Draw a string of letters at the specified row and column. The letters
            will fully define the ASCII values and color, i.e. the default mappings
            will not be used.

            Args:
                row: the row to use for writing. Must fall in the range [0, rows]
                col: the column to start writing at. Can be any value, and string
                     will be clipped appropriately
                letters: the values and colors to draw
        )gkdoc",
             "row"_a, "col"_a, "letters"_a)
        .def("draw", py::overload_cast<const Rect &, char>(&TextGrid::draw), R"gkdoc(
            Fills a rectangular area with the specified value

            Args:
                rect: the areaa to fill
                value: the ASCII value to use when filling
        )gkdoc",
             "rect"_a, "value"_a)
        .def("clear", py::overload_cast<Index, Index, Size>(&TextGrid::clear), R"gkdoc(
            Clears a region of the specified row (i.e. sets all characters to ' ').

            Args:
                row: the row to clear
                col: the starting column. Will be clipped appropriately
                cols: the number of columns to clear.
        )gkdoc",
             "row"_a, "col"_a, "cols"_a)
        .def("clear", py::overload_cast<const Rect &>(&TextGrid::clear), R"gkdoc(
            Clears a rectangular region (i.e. sets all characters to ' ').

            Args:
                rect: the region
        )gkdoc",
             "rect"_a)
        .def("get_letter", &TextGrid::get_letter, R"gkdoc(
            Get the ASCII value and color at the specified index.

            Args:
                row: the desired row
                column: the desired column
            
            Returns:
                the letter at this index
        )gkdoc",
             "row"_a, "col"_a)
        .def("get_row", &TextGrid::get_row, R"gkdoc(
            Get the desired Row.

            Args:
                row: must be a valid row index in the range [0, rows]
            
            Returns:
                a reference to the row
        )gkdoc",
             "row"_a)
        .def("__repr__", &TextGrid::to_string)
        .def_property_readonly("rows", &TextGrid::rows, "The number of rows in the grid")
        .def_property_readonly("cols", &TextGrid::cols, "The number of columns in the grid")
        .def_property_readonly("title", &TextGrid::title, R"gkdoc(
            The title of the grid. 
            
            This will be shown in the title bar of its window.
        )gkdoc");

    m.def("init", &init, "Manually initialize the library", R"gkdoc(
        Initializes the underlying OpenGL context. Pass any OS-specific parameters via
        this method. Will be called by default the first time start() is called otherwise.

        Args:
            args: the vector of OpenGL command line arguments
    )gkdoc",
          "args"_a = std::vector<std::string>());
    m.def("start", &start, "Start the GL event loop");
    m.def("stop", &stop, "Stop the GL event loop");
    m.def("create_grid", &create_grid, "rows"_a, "cols"_a, "title"_a = "Title", "default_color"_a = Colors::White);
    m.def("destroy_grid", &destroy_grid, "text_grid"_a, R"gkdoc(
        Destroys a text grid object.

        Args:
            text_grid: the grid to destroy
    )gkdoc");
    m.def("next_frame", &next_frame, R"gkdoc(
        Blocking call that waits for the next frame of animation.

        Args:
            frames_per_second: the target frame rate
    )gkdoc",
          "frames_per_second"_a = 30.0);
}