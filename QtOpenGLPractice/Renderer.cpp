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
#include "ViewPort.h"


extern std::string g_argument;

Renderer::Renderer()
{
    m_programMgr = new ProgramManager;
    m_fontMgr = new FontManager;
    m_viewport = new ViewPort;
    m_viewport->setDesign(-kDefaultDesignSize, -kDefaultDesignSize, kDefaultDesignSize * 2, kDefaultDesignSize * 2);
}

Renderer::~Renderer()
{
    delete m_viewport;
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
    glPointSize(5);
    return true;
}

void Renderer::resize(int width, int height)
{
    m_viewport->setScreen(static_cast<glint64>(width), static_cast<glint64>(height));
    m_viewport->updateViewToDesign();
}

void Renderer::render()
{
    m_start_time = std::chrono::steady_clock::now();
    glClearColor(kColorBG.r, kColorBG.g, kColorBG.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    legacyProjection();
}

void Renderer::endRender()
{
    std::chrono::steady_clock::time_point finish = std::chrono::steady_clock::now();
    std::chrono::microseconds result = std::chrono::duration_cast<std::chrono::microseconds>(finish - m_start_time);
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

ViewPort* Renderer::viewPort() const noexcept
{
    return m_viewport;
}

void Renderer::paintObject(GLObject2D* object) noexcept
{
    const Color3f& color = object->color();
    float opaque = object->opaque();
    ViewBox vb = toViewBox(m_viewport->getView());
    m_programMgr->applyProgram(ProgramType::BaseColor, color, opaque, &vb);
    object->draw();
    m_programMgr->releaseProgram();
}

void Renderer::paintFont(GLObject2D* object) noexcept
{
    const Color3f& color = object->color();
    float opaque = object->opaque();
    ViewBox vb = toViewBox(m_viewport->getView());
    m_programMgr->applyProgram(ProgramType::TextureFont, color, opaque, &vb);
    unsigned int texture_id = m_fontMgr->fontTexutre();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    object->drawAsFont();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_programMgr->releaseProgram();
}

ViewBox Renderer::toViewBox(const Box& box) noexcept
{
    ViewBox vb;
    vb.left = static_cast<float>(box.left());
    vb.bttm = static_cast<float>(box.bottom());
    vb.right = static_cast<float>(box.right());
    vb.top = static_cast<float>(box.top());
    return vb;
}

void Renderer::legacyProjection() noexcept
{
    ViewBox vb = toViewBox(m_viewport->getView());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(vb.left, vb.right, vb.bttm, vb.top, -1, 1);
}

