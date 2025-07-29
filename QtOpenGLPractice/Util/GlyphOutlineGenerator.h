#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Core/DefineStruct.h"


class GlyphOutlineGenerator
{
public:
    GlyphOutlineGenerator();

    ~GlyphOutlineGenerator();

    bool initFontLibrary(std::string& error_msg, const std::string & font_file = "") noexcept;

    void destroyFontLibrary() noexcept;

    void clearOutlines() noexcept;

    bool genGlyphOutlines(wchar_t ch, std::string& error_msg, GlyphOutlines& output) noexcept;

    // first step, discretize on local coordinate.
    static void discretizeOutline(const GlyphOutlines& outln, glint64 x, glint64 y, glint64 width, glint64 height, int res, std::vector<Point> &points) noexcept;

    static void discretizeConic(const Point& start, const Point& ctrl, const Point& end, int res, std::vector<Point>& points) noexcept;

    static void discretizeCubic(const Point& start, const Point& ctrl1, const Point& ctrl2, const Point& end, int res, std::vector<Point>& points) noexcept;

private:
    std::unordered_map<wchar_t, GlyphOutlines> m_glyph_outlines;
    struct Data;
    Data* m_data;
};


