#include "programmanager.h"
#include "ShaderSource.h"
#include "glad/glad.h"

ProgramManager::ProgramManager()
{
}

ProgramManager::~ProgramManager()
{
    for (const auto& [type, program] : m_programs)
    {
        glDeleteProgram(program);
    }
    m_programs.clear();
}

bool ProgramManager::initialize(std::string& err) noexcept
{
    unsigned int program = 0;
    if (!createProgram(kBaseColorVertexSource, kBaseColorFragmentSource, program, err))
    {
        return false;
    }
    m_programs[ProgramType::BaseColor] = program;
    if (!createProgram(kFontVertexSource, kFontFragmentSource, program, err))
    {
        return false;
    }
    m_programs[ProgramType::TextureFont] = program;
    return true;
}

unsigned int ProgramManager::program(ProgramType type) const noexcept
{
    return m_programs.at(type);
}

void ProgramManager::applyProgram(ProgramType type, const Color3f& color, unsigned char opaque, ViewBox* view) noexcept
{
    unsigned int program = this->program(type);
    glUseProgram(program);

    //be careful, if you want to change uniform in runtime, those code should between glUseProgram(m_program); updateUniform... glUseProgram(0);
    int nPosBaseColor = glGetUniformLocation(program, "baseColor");
    glUniform4f(nPosBaseColor, color.r, color.g, color.b, opaque);
    int nPosView = glGetUniformLocation(program, "view");
    glUniform4f(nPosView, view->left, view->right, view->bttm, view->top);
}

void ProgramManager::releaseProgram() noexcept
{
    glUseProgram(0);
}

bool ProgramManager::createProgram(const char* vert, const char* frag, unsigned int& program, std::string& errorMsg) noexcept
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
