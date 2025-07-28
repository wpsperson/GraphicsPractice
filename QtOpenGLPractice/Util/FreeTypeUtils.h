#pragma once

#include <vector>
#include <string>
#include <tuple>

#include "freetype/freetype.h"
#include "freetype/fttypes.h"
#include "freetype/ftoutln.h"
#include "freetype/ftsnames.h"


struct FontInfo
{
    std::string font_file;
    std::wstring font_name;
    bool chinese_language = false;
};

std::vector<std::string> allSystemFontFiles();

std::vector<FontInfo> getAllSystemFontInfos();

std::tuple<std::wstring, bool> findBestFontName(FT_Face face);

// test
void outputFontInfos();
