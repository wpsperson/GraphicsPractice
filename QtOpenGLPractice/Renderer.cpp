#include "Renderer.h"

#include <iostream>
#include <chrono>
#include <exception>
#include "glad/glad.h"
#include "GLObject2D.h"
#include "ProgramManager.h"
#include "Operation.h"
#include "DemoOperation.h"
#include "FontManager.h"

extern std::string g_argument;

Renderer::Renderer()
{
    m_programMgr = new ProgramManager;
    m_fontMgr = new FontManager;
    m_viewBox = new ViewBox;
}

Renderer::~Renderer()
{
    delete m_viewBox;
    delete m_fontMgr;
    delete m_programMgr;
}

bool Renderer::initialize(std::string& err)
{
    if (!gladLoadGL())
    {
        err = "failed to load glad!";
        return false;
    }
    if (!m_programMgr->initialize(err))
    {
        return false;
    }
    if (!m_fontMgr->loadAsciiCharInfo(err))
    {
        return false;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    injectAllOperation(this);
    for (Operation* oper : m_operations)
    {
        if (oper->name() == g_argument)
        {
            oper->initialize(this);
            break;
        }
    }
    return true;
}

void Renderer::resize(int width, int height)
{

}

void Renderer::render()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    glClearColor(kColorBG.r, kColorBG.g, kColorBG.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (Operation* oper : m_operations)
    {
        if (oper->name() == g_argument)
        {
            oper->paint(this);
            break;
        }
    }

    std::chrono::steady_clock::time_point finish = std::chrono::steady_clock::now();
    std::chrono::microseconds result = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
    double render_time = static_cast<double>(result.count()) / 1000.0;
    std::cout << "draw time(ms) is " << render_time << std::endl;
}

ProgramManager* Renderer::programMgr() noexcept
{
    return m_programMgr;
}

FontManager* Renderer::fontMgr() noexcept
{
    return m_fontMgr;
}

void Renderer::setViewBox(float left, float right, float bttm, float top) noexcept
{
    m_viewBox->left = left;
    m_viewBox->right = right;
    m_viewBox->bttm = bttm;
    m_viewBox->top = top;
}

ViewBox* Renderer::viewBox() const noexcept
{
    return m_viewBox;
}

void Renderer::addOperation(Operation* operation) noexcept
{
    m_operations.emplace_back(operation);
}

void Renderer::paintObject(GLObject2D* object) noexcept
{
    const Color3f& color = object->color();
    float opaque = object->opaque();
    m_programMgr->applyProgram(ProgramType::BaseColor, color, opaque, m_viewBox);
    object->draw();
    m_programMgr->releaseProgram();
}

void Renderer::paintFont(GLObject2D* object) noexcept
{
    const Color3f& color = object->color();
    float opaque = object->opaque();
    m_programMgr->applyProgram(ProgramType::TextureFont, color, opaque, m_viewBox);
    unsigned int texture_id = m_fontMgr->fontTexutre();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    object->drawAsFont();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_programMgr->releaseProgram();
}

