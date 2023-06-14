#pragma once

#include <map>
#include <vector>
#include <string>
#include "Const.h"

enum class ProgramType : unsigned char
{
    BaseColor = 0,
    TextureFont,
};

using AttribLoc = std::vector<std::pair<int, const char*>>;

class ProgramManager
{
public:
    ProgramManager();

    ~ProgramManager();

    bool initialize(std::string& err) noexcept;

    unsigned int program(ProgramType type) const noexcept;

    void applyProgram(ProgramType type, const Color3f &color, float opaque, ViewBox *view) noexcept;

    void releaseProgram() noexcept;

private:
    bool createProgram(const char* vs, const char* fs, const AttribLoc &attribs, unsigned int& program, std::string& err) noexcept;

private:
    std::map<ProgramType, unsigned int> m_programs;
};

