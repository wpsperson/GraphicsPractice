#pragma once

#include <corecrt_math_defines.h>
#include <vector>

using glint64 = long long;

struct llPoint
{
    glint64 x;
    glint64 y;
};

struct Color3f
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

struct ViewBox
{
    float left = -1.0f;
    float right = 1.0f;
    float bttm = -1.0f;
    float top = 1.0f;
};

struct AsciiCharInfo
{
    float width = 0.0f;
    float height = 0.0f;
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    float stride = 0.0f;

    float uv_min_x = 0.0f;
    float uv_max_x = 0.0f;
    float uv_min_y = 0.0f;
    float uv_max_y = 0.0f;
};

struct Point
{
    float x = 0.0f;
    float y = 0.0f;
    Point() {}
    Point(float _x, float _y): x(_x), y(_y)
    {}

    void scale(float sx, float sy)
    {
        x *= sx;
        y *= sy;
    }

    void move(float offx, float offy)
    {
        x += offx;
        y += offy;
    }
};

struct Color4uc
{
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;
    unsigned char alpha = 0;
};

struct ColorVertex
{
    Point point; 
    Color4uc color;
    ColorVertex() = default;
    ColorVertex(const Point& pt, Color4uc col) : point(pt), color(col) {}
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

struct GlyphLoop
{
    std::vector<SVGCmd> cmds;
    std::vector<Point> coords;
};

struct GlyphOutlines
{
    int units_per_EM;
    std::vector<GlyphLoop> loops;
};

constexpr glint64 kDefaultDesignSize = 1000000;
constexpr Color3f kColorRed = { 1.0f, 0.0f, 0.0f };
constexpr Color3f kColorGreen = { 0.0f, 1.0f, 0.0f };
constexpr Color3f kColorBlue = { 0.0f, 0.0f, 1.0f };
constexpr Color3f kColorWhite = { 1.0f, 1.0f, 1.0f };
constexpr Color3f kColorBlack = { 0.0f, 0.0f, 0.0f };
constexpr Color3f kColorGray = { 0.5f, 0.5f, 0.5f };
constexpr Color3f kColorCyan = { 0.0f, 1.0f, 1.0f };
constexpr Color3f kColorMagenta = { 1.0f, 0.0f, 1.0f };
constexpr Color3f kColorYellow = { 1.0f, 1.0f, 0.0f };
constexpr Color3f kColorBG = { 0.3f, 0.4f, 0.5f };
