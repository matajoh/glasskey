""" glasskey module """

from ._glasskey import init, start, stop, create_grid, destroy_grid, Color,\
    Letter, Rect, TextGrid
from . import _glasskey

class Colors:
    """Some standard colors."""
    Black = _glasskey.Black
    White = _glasskey.White
    Red = _glasskey.Red
    Maroon = _glasskey.Maroon
    Pink = _glasskey.Pink
    Brown = _glasskey.Brown
    Orange = _glasskey.Orange
    Coral = _glasskey.Coral
    Olive = _glasskey.Olive
    Yellow = _glasskey.Yellow
    Beige = _glasskey.Beige
    Lime = _glasskey.Lime
    Green = _glasskey.Green
    Mint = _glasskey.Mint
    Teal = _glasskey.Teal
    Cyan = _glasskey.Cyan
    Navy = _glasskey.Navy
    Blue = _glasskey.Blue
    Purple = _glasskey.Purple
    Lavender = _glasskey.Lavender
    Magenta = _glasskey.Magenta
    Gray = _glasskey.Gray
