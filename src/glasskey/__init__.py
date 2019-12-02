""" glasskey module """

from ._pyglasskey import init, start, stop, create_grid, destroy_grid, Color,\
    next_frame, Letter, Rect, TextGrid
from . import _pyglasskey

class Colors:
    """Some standard colors."""
    Black = _pyglasskey.Black
    White = _pyglasskey.White
    Red = _pyglasskey.Red
    Maroon = _pyglasskey.Maroon
    Pink = _pyglasskey.Pink
    Brown = _pyglasskey.Brown
    Orange = _pyglasskey.Orange
    Coral = _pyglasskey.Coral
    Olive = _pyglasskey.Olive
    Yellow = _pyglasskey.Yellow
    Beige = _pyglasskey.Beige
    Lime = _pyglasskey.Lime
    Green = _pyglasskey.Green
    Mint = _pyglasskey.Mint
    Teal = _pyglasskey.Teal
    Cyan = _pyglasskey.Cyan
    Navy = _pyglasskey.Navy
    Blue = _pyglasskey.Blue
    Purple = _pyglasskey.Purple
    Lavender = _pyglasskey.Lavender
    Magenta = _pyglasskey.Magenta
    Gray = _pyglasskey.Gray
