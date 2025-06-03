#include "Utils.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>


void parseFontFile(const char* file, int font_size, VFontLibrary &lib)
{
    std::ifstream ifs(file);
    if (ifs.fail())
    {
        return;
    }
    lib.resize(128);
    std::string header;
    std::string strCommand;
    int line_count = 0;
    char ch = 0;

    int cmd = 0;
    int ptx = 0;
    int pty = 0;
    while (true)
    {
        std::getline(ifs, header);
        std::stringstream ss(header);
        ss >> line_count >> ch;
        VFont &font = lib[ch];
        font.ch = ch;
        for (int i = 0; i < line_count; i++)
        {
            std::getline(ifs, strCommand);
            std::stringstream stream(strCommand);
            stream >> cmd >> ptx >> pty;
            if (cmd == 1)
            {
                VStroke sk;
                sk.cmds.emplace_back(1);
                sk.points.emplace_back(ptx, pty, font_size);
                font.strokes.emplace_back(sk);
            }
            else if(cmd == 2 || cmd ==3 || cmd == 4)
            {
                VStroke &sk = font.strokes.back();
                sk.cmds.emplace_back(cmd);
                sk.points.emplace_back(ptx, pty, font_size);
            }
        }

        if (ch == '~')
        {
            break;
        }
    }
}

namespace
{
    int lineCount(const VFont& font)
    {
        int ret = 0;
        for (const VStroke sk : font.strokes)
        {
            ret += static_cast<int>(sk.points.size());
        }
        return ret;
    }
}

void writeFontFile(const char* file, const VFontLibrary& lib)
{
    std::ofstream ofs(file);
    if (ofs.fail())
    {
        return;
    }
    for (int i = 33; i <= 126; i++)
    {
        const VFont& font = lib[i];
        unsigned char ch = font.ch;
        int line_count = lineCount(font);
        ofs << line_count << "            " << ch << std::endl;

        for (const VStroke sk : font.strokes)
        {
            int pt_count = static_cast<int>(sk.points.size());
            for (int i = 0; i < pt_count; i++)
            {
                int cmd = sk.cmds[i];
                const VPoint& pt = sk.points[i];
                ofs << cmd << " " << pt.x << " " << pt.y << std::endl;
            }
        }
    }
    ofs.close();
}

void writeFontFileInNewFormat(const char* file, const VFontLibrary& lib)
{
    std::ofstream ofs(file);
    if (ofs.fail())
    {
        return;
    }
    for (int i = 33; i <= 126; i++)
    {
        const VFont& font = lib[i];
        unsigned char ch = font.ch;
        int line_count = static_cast<int>(font.strokes.size());
        ofs << line_count << "            " << ch << std::endl;

        for (const VStroke sk : font.strokes)
        {
            int pt_count = static_cast<int>(sk.points.size());
            for (int i = 0; i < pt_count; i++)
            {
                const VPoint& pt = sk.points[i];
                ofs << pt.x << " " << pt.y;
                if (i != pt_count - 1)
                {
                    ofs << ", ";
                }
            }
            ofs << std::endl;
        }
    }
    ofs.close();
}

std::string timeToStr(const std::string& format/* = "%Y-%m-%d %H-%M-%S"*/)
{
    time_t t = std::time(nullptr);
    //tm _tm = *std::localtime_s(&t);
    tm _tm;
    localtime_s(&_tm, &t);
    std::ostringstream oss;
    oss << std::put_time(&_tm, format.data());
    std::string str_time = oss.str();
    return str_time;
}

Point toFPoint(const llPoint& pt)
{
    return Point(static_cast<float>(pt.x), static_cast<float>(pt.y));
}

ViewBox Utils::toViewBox(const Box& box) noexcept
{
    ViewBox vb;
    vb.left = static_cast<float>(box.left());
    vb.bttm = static_cast<float>(box.bottom());
    vb.right = static_cast<float>(box.right());
    vb.top = static_cast<float>(box.top());
    return vb;
}
