#pragma once

#include <string>
#include <unordered_map>
#include "Const.h"

class FontManager
{
public:
    FontManager();

    ~FontManager();

    unsigned int fontTexutre() const noexcept;

    const AsciiCharInfo* asciiChar(char ch) const noexcept;

    bool loadAsciiCharInfo(std::string& error) noexcept;

private:
    unsigned int m_textureID = 0;
    std::unordered_map<char, AsciiCharInfo> m_fonts;
};


