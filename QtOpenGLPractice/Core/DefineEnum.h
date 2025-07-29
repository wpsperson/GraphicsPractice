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