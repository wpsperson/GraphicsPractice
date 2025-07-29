#pragma once

#include <map>
#include <vector>
#include <string>
#include "Core/DefineStruct.h"

enum class ProgramType : unsigned char
{
    BaseColor = 0,
    TextureFont,
    Texture2D,
    ColorVertex,
    PolygonStipple,
    LineStipple,
};

using AttribLoc = std::vector<std::pair<int, const char*>>;

class ProgramManager
{
public:
    ProgramManager();

    ~ProgramManager();

    bool initialize(std::string& err) noexcept;

    unsigned int program(ProgramType type) const noexcept;

    void applyProgram(ProgramType type) noexcept;

    void uniformViewBox(const ViewBox& view) noexcept;

    void uniform1f(std::string_view name, float value0) noexcept;
    void uniform2f(std::string_view name, float value0, float value1) noexcept;
    void uniform1ui(std::string_view name, unsigned int value0) noexcept;

    void applyProgram(ProgramType type, const Color3f &color, float opaque, ViewBox *view) noexcept;

    void releaseProgram() noexcept;

private:
    bool createProgram(const char* vs, const char* fs, const AttribLoc &attribs, unsigned int& program, std::string& err) noexcept;

private:
    std::map<ProgramType, unsigned int> m_programs;
    unsigned int m_current_program = 0;
};

