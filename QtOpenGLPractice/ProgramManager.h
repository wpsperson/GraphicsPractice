#pragma once

#include <map>
#include <string>
#include "Const.h"

enum class ProgramType : unsigned char
{
    BaseColor = 0,
    TextureFont,
};

class ProgramManager
{
public:
    ProgramManager();

    ~ProgramManager();

    bool initialize(std::string& err) noexcept;

    unsigned int program(ProgramType type) const noexcept;

    void applyProgram(ProgramType type, const Color3f &color, unsigned char opaque, ViewBox *view) noexcept;

    void releaseProgram() noexcept;

private:
    bool createProgram(const char* vs, const char* fs, unsigned int& program, std::string& err) noexcept;

private:
    std::map<ProgramType, unsigned int> m_programs;
};

