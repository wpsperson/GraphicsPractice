#pragma once

#include <vector>
#include <cmath>
#include <string>

const int kOldFontDim = 32767;
const int kNewFontDim = 100;
const int kViewSize = 1000;
const int kScreenSize = 2000;

struct VPoint
{
    int x;
    int y;
    int dim;
    double normx() const
    {
        return double(x) / dim;
    }
    double normy() const
    {
        return double(y) / dim;
    }
    void setNorm(double normx, double normy)
    {
        x = std::lround(normx * dim);
        y = std::lround(normy * dim);
    }
};

struct VStroke
{
    std::vector<int> cmds; // 1 moveto;2hori to;3vert to; 4lineto
    std::vector<VPoint> points;
};

struct VFont
{
    unsigned char ch;
    std::vector<VStroke> strokes;
};

using VFontLibrary = std::vector<VFont>;

void parseFontFile(const char* file, int font_size, VFontLibrary &lib);

void writeFontFile(const char* file, const VFontLibrary& lib);

void writeFontFileInNewFormat(const char* file, const VFontLibrary& lib);

std::string timeToStr(const std::string& format = "%Y-%m-%d %H-%M-%S");