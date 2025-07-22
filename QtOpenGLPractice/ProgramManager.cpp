#include "programmanager.h"
#include "ShaderSource.h"
#include "OpenGLHeader.h"

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
    AttribLoc basicAttribs;
    basicAttribs.emplace_back(std::make_pair(0, "aPos"));
    if (!createProgram(kBaseColorVertexSource, kBaseColorFragmentSource, basicAttribs, program, err))
    {
        return false;
    }
    m_programs[ProgramType::BaseColor] = program;

    AttribLoc fontAttribs;
    fontAttribs.emplace_back(std::make_pair(0, "aPos"));
    fontAttribs.emplace_back(std::make_pair(1, "aUV"));
    if (!createProgram(kFontVertexSource, kFontFragmentSource, fontAttribs, program, err))
    {
        return false;
    }
    //GLint aPos = glGetAttribLocation(program, "aPos");
    //GLint aUV = glGetAttribLocation(program, "aUV");
    m_programs[ProgramType::TextureFont] = program;

    if (!createProgram(kFontVertexSource, kTextureFragmentSource, fontAttribs, program, err))
    {
        return false;
    }
    m_programs[ProgramType::Texture2D] = program;

    if (!createProgram(kColorVertexVS, kColorVertexFS, AttribLoc{}, program, err))
    {
        return false;
    }
    m_programs[ProgramType::ColorVertex] = program;

    return true;
}

unsigned int ProgramManager::program(ProgramType type) const noexcept
{
    return m_programs.at(type);
}

void ProgramManager::applyProgram(ProgramType type) noexcept
{
    unsigned int program = this->program(type);
    glUseProgram(program);
    m_current_program = program;
}

void ProgramManager::uniformViewBox(const ViewBox& view) noexcept
{
    int nPosView = glGetUniformLocation(m_current_program, "view");
    glUniform4f(nPosView, view.left, view.right, view.bttm, view.top);
}

void ProgramManager::applyProgram(ProgramType type, const Color3f& color, float opaque, ViewBox* view) noexcept
{
    unsigned int program = this->program(type);
    glUseProgram(program);
    m_current_program = program;

    //be careful, if you want to change uniform in runtime, those code should between glUseProgram(m_program); updateUniform... glUseProgram(0);
    int nPosBaseColor = glGetUniformLocation(program, "baseColor");
    glUniform4f(nPosBaseColor, color.r, color.g, color.b, opaque);
    int nPosView = glGetUniformLocation(program, "view");
    glUniform4f(nPosView, view->left, view->right, view->bttm, view->top);

}

void ProgramManager::releaseProgram() noexcept
{
    glUseProgram(0);
    m_current_program = 0;
}

bool ProgramManager::createProgram(const char* vert, const char* frag, const AttribLoc& attribs, unsigned int& program, std::string& errorMsg) noexcept
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
    for (const auto& [pos, name] : attribs)
    {
        glBindAttribLocation(shaderProgram, pos, name);
    }
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
