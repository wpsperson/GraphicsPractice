#pragma once

#include <array>
#include <string>

class ProgramManager
{
public:
    ProgramManager();
    ~ProgramManager();

    void applyProgram(bool useNormal, bool useTexture, const std::array<float, 16>& model, const std::array<float, 16>& view, const std::array<float, 16>& projection);

    unsigned int vertexProgram();

    unsigned int vertexNormalProgram();

    unsigned int vertexUVProgram();

    unsigned int vertexNormalUVProgram();

private:
    bool createProgramByStr(const char* vert, const char* frag, unsigned int &program, std::string &errorMsg);

private:
    unsigned int m_vertexProgram = 0;
    unsigned int m_vertexNormalProgram = 0;
    unsigned int m_vertexUVProgram = 0;
    unsigned int m_vertexNormalUVProgram = 0;
};