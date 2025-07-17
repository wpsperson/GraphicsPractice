#include "Renderer.h"

#include <iostream>
#include <chrono>
#include <exception>
#include "OpenGLHeader.h"
#include "GLObject2D.h"
#include "ProgramManager.h"
#include "FontManager.h"
#include "ViewPort.h"
#include "Utils.h"
#include "Operations/Operation.h"
#include "Operations/DemoOperation.h"


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
    GLenum GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        const char* error_message = reinterpret_cast<const char*>(glewGetErrorString(GlewInitResult));
        if (error_message)
        {
            std::cerr << error_message << std::endl;
        }
        exit(EXIT_FAILURE);
    }
    if (!Utils::getOpenGLVersion(m_major_version, m_minor_version))
    {
        throw std::exception("Fail to read OpenGL version!");
        std::exit(EXIT_FAILURE);
    }
    if (!GLEW_VERSION_2_0)
    {
        throw std::exception("Required OpenGL 2.0 Features are not available!");
        std::exit(EXIT_FAILURE);
    }
    

    if (!m_programMgr->initialize(err))
    {
        return false;
    }
    if (!m_fontMgr->loadAsciiCharInfo(err))
    {
        return false;
    }

    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(5);
    glEnable(GL_TEXTURE);
    return true;
}

void Renderer::resize(int width, int height)
{
    m_viewport->setScreen(static_cast<glint64>(width), static_cast<glint64>(height));
    m_viewport->updateViewToDesign();
}

void Renderer::beforeRender()
{
    m_start_time = std::chrono::steady_clock::now();
    glClearColor(kColorBG.r, kColorBG.g, kColorBG.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // legacyProjection();
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

std::pair<int, int> Renderer::getVersion() const noexcept
{
    return std::pair<int, int>(m_major_version, m_minor_version);
}

void Renderer::paintObject(GLObject2D* object) noexcept
{
    const Color3f& color = object->color();
    float opaque = object->opaque();
    ViewBox vb = Utils::toViewBox(m_viewport->getView());
    m_programMgr->applyProgram(ProgramType::BaseColor, color, opaque, &vb);
    object->draw();
    m_programMgr->releaseProgram();
}

void Renderer::paintFont(GLObject2D* object) noexcept
{
    const Color3f& color = object->color();
    float opaque = object->opaque();
    ViewBox vb = Utils::toViewBox(m_viewport->getView());
    m_programMgr->applyProgram(ProgramType::TextureFont, color, opaque, &vb);
    unsigned int texture_id = m_fontMgr->fontTexutre();
    glBindTexture(GL_TEXTURE_2D, texture_id);
    object->drawAsFont();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_programMgr->releaseProgram();
}

void Renderer::legacyProjection() noexcept
{
    ViewBox vb = Utils::toViewBox(m_viewport->getView());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(vb.left, vb.right, vb.bttm, vb.top, -1, 1);
}

void Renderer::beginStencil()
{
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
}

void Renderer::endStencil()
{
    glDisable(GL_STENCIL_TEST);
}

void Renderer::stencilFill()
{
    glStencilFunc(GL_GREATER, 1, 0xff);
}

void Renderer::stencilText()
{
    glStencilFunc(GL_EQUAL, 1, 0xff);
}

void Renderer::stencilVoidBegin()
{
    glStencilFunc(GL_GREATER, 2, 0xff);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void Renderer::stencilVoidEnd()
{
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}


