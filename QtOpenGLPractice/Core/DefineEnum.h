#pragma once


enum class PolyStipple : unsigned char
{
    Solid = 0,
    PointHatch,
    Count,
};

enum class LineStipple : unsigned char
{
    Solid = 0,
    Hidden,
    Phantom,
    Dotted,
    Center,
    Dashed,
    Count,
};

enum class DrawMode : unsigned char
{
    Points = 0x00,
    Lines = 0x01,
    Fill = 0x04,
};

enum class SVGCmd : unsigned char
{
    MoveTo = 0,
    LineTo,
    Conic,
    Cubic,
};
