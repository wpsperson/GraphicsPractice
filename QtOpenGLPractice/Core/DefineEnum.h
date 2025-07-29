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
    Fill = 0,
    Lines,
    Points,
};

enum class SVGCmd : unsigned char
{
    MoveTo = 0,
    LineTo,
    Conic,
    Cubic,
};
