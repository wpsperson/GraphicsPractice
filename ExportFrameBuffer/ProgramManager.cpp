#include "ProgramManager.h"

#include <iostream>
#include "glad/glad.h"

#include "ShaderDefine.h"


ProgramManager::ProgramManager()
{
}

ProgramManager::~ProgramManager()
{
    if (m_vertexProgram)
    {
        glDeleteProgram(m_vertexProgram);
    }
    if (m_vertexNormalProgram)
    {
        glDeleteProgram(m_vertexNormalProgram);
    }
    if (m_vertexUVProgram)
    {
        glDeleteProgram(m_vertexUVProgram);
    }
    if (m_vertexNormalUVProgram)
    {
        glDeleteProgram(m_vertexNormalUVProgram);
    }
}

void ProgramManager::applyProgram(bool useNormal, bool useTexture, const std::array<float, 16>& model, const std::array<float, 16>& view, const std::array<float, 16>& projection)
{
    unsigned int program = 0;
    if (useNormal && useTexture)
    {
        program = vertexNormalUVProgram();
        glUseProgram(program);
        int locDiffuseTexture = glGetUniformLocation(program, "diffuseTexture");
        glUniform1i(locDiffuseTexture, 0);
        int locViewPos = glGetUniformLocation(program, "viewPos");
        glUniform3f(locViewPos, 0.0f, 0.0f, 0.0f);
        float sqrt3_3 = static_cast<float>(sqrt(3.0) / 3);
        int locLightDir = glGetUniformLocation(program, "lightDir");
        glUniform3f(locLightDir, sqrt3_3, sqrt3_3, sqrt3_3);
        int locBaseColor = glGetUniformLocation(program, "baseColor");
        glUniform4f(locBaseColor, 1.0f, 1.0f, 1.0f, 1.0f);
    }
    else if (useNormal)
    {
        program = vertexNormalProgram();
        glUseProgram(program);
        int locViewPos = glGetUniformLocation(program, "viewPos");
        glUniform3f(locViewPos, 0.0f, 0.0f, 0.0f);
        float sqrt3_3 = static_cast<float>(sqrt(3.0) / 3);
        int locLightDir = glGetUniformLocation(program, "lightDir");
        glUniform3f(locLightDir, sqrt3_3, sqrt3_3, sqrt3_3);
        int locBaseColor = glGetUniformLocation(program, "baseColor");
        glUniform4f(locBaseColor, 1.0f, 1.0f, 1.0f, 1.0f);
    }
    else if (useTexture)
    {
        program = vertexUVProgram();
        glUseProgram(program);
        int locDiffuseTexture = glGetUniformLocation(program, "diffuseTexture");
        glUniform1i(locDiffuseTexture, 0);
    }
    else
    {
        program = vertexProgram();
        glUseProgram(program);
        int locBaseColor = glGetUniformLocation(program, "baseColor");
        glUniform4f(locBaseColor, 1.0f, 1.0f, 1.0f, 1.0f);
    }

    int locModel = glGetUniformLocation(program, "model");
    int locView = glGetUniformLocation(program, "view");
    int locProjection = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, model.data());
    glUniformMatrix4fv(locView, 1, GL_FALSE, view.data());
    glUniformMatrix4fv(locProjection, 1, GL_FALSE, projection.data());
}

unsigned int ProgramManager::vertexProgram()
{
    if (m_vertexProgram == 0)
    {
        std::string errorMsg;
        if (!createProgramByStr(strVertex, strFragment, m_vertexProgram, errorMsg))
        {
            std::cout << errorMsg << std::endl;
        }
    }
    return m_vertexProgram;
}

unsigned int ProgramManager::vertexNormalProgram()
{
    if (m_vertexNormalProgram == 0)
    {
        std::string errorMsg;
        if (!createProgramByStr(strVertexForNormal, strFragmentForNormal, m_vertexNormalProgram, errorMsg))
        {
            std::cout << errorMsg << std::endl;
        }
    }
    return m_vertexNormalProgram;
}

unsigned int ProgramManager::vertexUVProgram()
{
    if (m_vertexUVProgram == 0)
    {
        std::string errorMsg;
        if (!createProgramByStr(strVertexForTexture, strFragmentForTexture, m_vertexUVProgram, errorMsg))
        {
            std::cout << errorMsg << std::endl;
        }
    }
    return m_vertexUVProgram;
}

unsigned int ProgramManager::vertexNormalUVProgram()
{
    if (m_vertexNormalUVProgram == 0)
    {
        std::string errorMsg;
        if (!createProgramByStr(strVertexForNormalTexture, strFragmentForNormalTexture, m_vertexNormalUVProgram, errorMsg))
        {
            std::cout << errorMsg << std::endl;
        }
    }
    return m_vertexNormalUVProgram;
}

bool ProgramManager::createProgramByStr(const char* vert, const char* frag, unsigned int& program, std::string& errorMsg)
{
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vert, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        errorMsg = "Failed to compile vertex shader: " + std::string(infoLog);
        return false;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &frag, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        errorMsg = "Failed to compile fragment shader: " + std::string(infoLog);
        return false;
    }
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        errorMsg = "Failed to link program: " + std::string(infoLog);
        return false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    program = shaderProgram;
    return true;
}
