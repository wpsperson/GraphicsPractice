#include "GlyphOutlineGenerator.h"

#include <cassert>
#include "freetype/freetype.h"
#include "freetype/fttypes.h"
#include "freetype/ftoutln.h"
#pragma warning(disable: 4996)

struct GlyphOutlineGenerator::Data
{
    FT_Library library_;
    FT_Face face_;
};
namespace
{
    int moveToFunction(const FT_Vector* kTo, void* user)
    {
        GlyphOutlines* self = static_cast<GlyphOutlines*>(user);
        float units = static_cast<float>(self->units_per_EM);

        GlyphLoop new_outln;
        new_outln.cmds.emplace_back(SVGCmd::MoveTo);
        new_outln.coords.emplace_back(kTo->x / units, kTo->y / units);
        self->loops.emplace_back(new_outln);
        return 0;
    }

    int lineToFunction(const FT_Vector* kTo, void* user)
    {
        GlyphOutlines* self = static_cast<GlyphOutlines*>(user);
        float units = static_cast<float>(self->units_per_EM);
        GlyphLoop& current = self->loops.back();
        current.cmds.emplace_back(SVGCmd::LineTo);
        current.coords.emplace_back(kTo->x / units, kTo->y / units);
        return 0;
    }

    int conicToFunction(const FT_Vector* ctrl, const FT_Vector* kTo, void* user)
    {
        GlyphOutlines* self = static_cast<GlyphOutlines*>(user);
        float units = static_cast<float>(self->units_per_EM);
        GlyphLoop& current = self->loops.back();
        current.cmds.emplace_back(SVGCmd::Conic);
        current.coords.emplace_back(ctrl->x / units, ctrl->y / units);
        current.coords.emplace_back(kTo->x / units, kTo->y / units);
        return 0;
    }

    int cubicToFunction(const FT_Vector* ctrl1, const FT_Vector* ctrl2, const FT_Vector* kTo, void* user)
    {
        GlyphOutlines* self = static_cast<GlyphOutlines*>(user);
        float units = static_cast<float>(self->units_per_EM);
        GlyphLoop& current = self->loops.back();
        current.cmds.emplace_back(SVGCmd::Cubic);
        current.coords.emplace_back(ctrl1->x / units, ctrl1->y / units);
        current.coords.emplace_back(ctrl2->x / units, ctrl2->y / units);
        current.coords.emplace_back(kTo->x / units, kTo->y / units);
        return 0;
    }
}


GlyphOutlineGenerator::GlyphOutlineGenerator()
{
    m_data = new Data;
}

GlyphOutlineGenerator::~GlyphOutlineGenerator()
{
    destroyFontLibrary();
    delete m_data;
}

bool GlyphOutlineGenerator::initFontLibrary(std::string& error_msg, const std::string& font_file) noexcept
{
    FT_Error error = FT_Init_FreeType(&m_data->library_);
    if (error)
    {
        error_msg = "Failed to init Free Type.";
        return false;
    }

    std::string font_file_path;
    if (font_file.empty())
    {
        font_file_path = std::getenv("SystemRoot") + std::string("/Fonts/") + "Arial.ttf";
    }
    else
    {
        font_file_path = font_file;
    }
    error = FT_New_Face(m_data->library_, font_file_path.c_str(), 0, &m_data->face_);
    if (error)
    {
        error_msg = "Failed to create new face.";
    }
    return true;
}

void GlyphOutlineGenerator::destroyFontLibrary() noexcept
{
    FT_Done_Face(m_data->face_);
    FT_Done_FreeType(m_data->library_);
}

void GlyphOutlineGenerator::clearOutlines() noexcept
{
    m_glyph_outlines.clear();
}

bool GlyphOutlineGenerator::genGlyphOutlines(wchar_t ch, std::string& error_msg, GlyphOutlines& output) noexcept
{
    if (m_glyph_outlines.count(ch))
    {
        output = m_glyph_outlines.at(ch);
        return true;
    }

    FT_UInt index = FT_Get_Char_Index(m_data->face_, ch);
    FT_Error error = FT_Load_Glyph(m_data->face_, index, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);

    if (error)
    {
        error_msg = "Failed to load the glyph.";
        return false;
    }

    FT_Outline& outline = m_data->face_->glyph->outline;
    error = FT_Outline_Check(&outline);
    if (error)
    {
        error_msg = "Failed to check the outline.";
        return false;
    }

    //FT_Fixed multiple = 65536L;
    //FT_Matrix matrix;
    //matrix.xx = 1L * multiple;
    //matrix.xy = 0L * multiple;
    //matrix.yx = 0L * multiple;
    //matrix.yy = -1L * multiple;
    //FT_Outline_Transform(&outline, &matrix);

    FT_Outline_Funcs callbacks;
    callbacks.move_to = moveToFunction;
    callbacks.line_to = lineToFunction;
    callbacks.conic_to = conicToFunction;
    callbacks.cubic_to = cubicToFunction;
    callbacks.shift = 0;
    callbacks.delta = 0;

    GlyphOutlines result;
    result.units_per_EM = m_data->face_->units_per_EM;
    error = FT_Outline_Decompose(&outline, &callbacks, &result);
    if (error)
    {
        error_msg = "Failed to decompose the outline.";
        return false;
    }
    //FT_BBox box;
    //FT_Outline_Get_CBox(&outline, &box);
    output = result;
    m_glyph_outlines[ch] = result;
    return true;
}

void GlyphOutlineGenerator::discretizeOutline(const GlyphOutlines& outlines, glint64 x, glint64 y, glint64 width, glint64 height, int res, std::vector<Point>& points) noexcept
{
    points.clear();
    float offx = static_cast<float>(x);
    float offy = static_cast<float>(y);
    float scalex = static_cast<float>(width);
    float scaley = static_cast<float>(height);
    Point last;
    for (const GlyphLoop& outln : outlines.loops)
    {
        std::vector<Point>::const_iterator itr = outln.coords.cbegin();
        for (SVGCmd cmd : outln.cmds)
        {
            if (SVGCmd::MoveTo == cmd)
            {
                last = *itr++;
                last.scale(scalex, scaley);
                last.move(offx, offy);
            }
            else if (SVGCmd::LineTo == cmd)
            {
                Point end = *itr++;
                end.scale(scalex, scaley);
                end.move(offx, offy);
                points.emplace_back(last);
                points.emplace_back(end);
                last = end;
            }
            else if (SVGCmd::Conic == cmd)
            {
                Point ctrl = *itr++;
                Point end = *itr++;
                ctrl.scale(scalex, scaley);
                end.scale(scalex, scaley);
                ctrl.move(offx, offy);
                end.move(offx, offy);
                discretizeConic(last, ctrl, end, res, points);
                last = end;
            }
            else if (SVGCmd::Cubic == cmd)
            {
                Point ctrl1 = *itr++;
                Point ctrl2 = *itr++;
                Point end = *itr++;
                ctrl1.scale(scalex, scaley);
                ctrl2.scale(scalex, scaley);
                end.scale(scalex, scaley);
                ctrl1.move(offx, offy);
                ctrl2.move(offx, offy);
                end.move(offx, offy);
                discretizeCubic(last, ctrl1, ctrl2, end, res, points);
                last = end;
            }
            else
            {
                assert(false);
            }
        }
    }
}

void GlyphOutlineGenerator::discretizeConic(const Point& start, const Point& ctrl, const Point& end, int res, std::vector<Point>& points) noexcept
{
    double delta = 1.0 / (res - 1);
    Point pt;
    for (int i = 0; i < res; i++)
    {
        double t = i * delta;
        double x = pow(1 - t, 2) * start.x + 2 * t * (1 - t) * ctrl.x + pow(t, 2) * end.x;
        double y = pow(1 - t, 2) * start.y + 2 * t * (1 - t) * ctrl.y + pow(t, 2) * end.y;
        pt.x = static_cast<float>(x);
        pt.y = static_cast<float>(y);
        points.emplace_back(pt);
        if (i != 0 && i != res - 1)
        {
            points.emplace_back(pt);
        }
    }
}

void GlyphOutlineGenerator::discretizeCubic(const Point& start, const Point& ctrl1, const Point& ctrl2, const Point& end, int res, std::vector<Point>& points) noexcept
{
    double delta = 1.0 / (res - 1);
    Point pt;
    for (int i = 0; i < res; i++)
    {
        double t = i * delta;
        double x = pow(1 - t, 3) * start.x + 3 * t * pow(1 - t, 2) * ctrl1.x + 3 * t * t * (1 - t) * ctrl2.x + pow(t, 3) * end.x;
        double y = pow(1 - t, 3) * start.y + 3 * t * pow(1 - t, 2) * ctrl1.y + 3 * t * t*  (1 - t) * ctrl2.y + pow(t, 3) * end.y;
        pt.x = static_cast<float>(x);
        pt.y = static_cast<float>(y);
        points.emplace_back(pt);
        if (i != 0 && i != res - 1)
        {
            points.emplace_back(pt);
        }
    }
}

