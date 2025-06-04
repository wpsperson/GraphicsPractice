#include "Operations/FBOOperation.h"

#include <iostream>
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QMouseEvent>

#include "Renderer.h"
#include "OpenGLHeader.h"
#include "ViewPort.h"
#include "Utils.h"
#include "ProgramManager.h"
#include "OpenGLWidget.h"

FBOOperation::FBOOperation()
{
}

FBOOperation::~FBOOperation()
{
    if (m_static)
    {
        delete m_static;
    }
    if (m_dynamic)
    {
        delete m_dynamic;
    }
    if (m_tex_obj)
    {
        delete m_tex_obj;
    }
    if (m_texture)
    {
        glDeleteTextures(1, &m_texture);
    }
    if (m_fbo)
    {
        glDeleteFramebuffers(1, &m_fbo);
    }
}

void FBOOperation::initialize(Renderer* renderer) noexcept
{
    m_renderer = renderer;
}

void FBOOperation::paint(Renderer* renderer) noexcept
{
    // first time
    if (0 == m_fbo)
    {
        initFBO();
    }
    if (!m_texture_done)
    {
        std::cout << "drawStaticScene...." << std::endl;
        drawStaticScene();
        m_texture_done = true;
    }

    // loop
    if (m_texture_done)
    {
        std::cout << "draw texture and dynamic...." << std::endl;
        applyTexture();
        drawDynamicScene();
    }
    else
    {
        std::cout << "m_texture_done is not true!" << std::endl;
    }
}

void FBOOperation::processMouseClick(QMouseEvent* eve)
{
    if (eve->button() == Qt::LeftButton)
    {
        m_drag = true;
        m_widget->update();
    }
    else if (eve->button() == Qt::RightButton)
    {
        m_widget->update();
    }
}

void FBOOperation::processMouseRelease(QMouseEvent* eve)
{
    if (eve->button() == Qt::LeftButton)
    {
        m_drag = false;
        m_widget->update();
    }
}

void FBOOperation::processMouseMove(QMouseEvent* eve)
{
    int x = eve->pos().x();
    int y = eve->pos().y();
    m_mouse_pt = m_renderer->viewPort()->screenToDB(x, y);
    if (m_drag)
    {
        m_widget->update();
    }
}

void FBOOperation::processMouseWheel(QWheelEvent* eve)
{
    std::cout << "processMouseWheel...." << std::endl;
    m_texture_done = false;
}

bool FBOOperation::initFBO()
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Fail to complete frame buffer!" << std::endl;
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void FBOOperation::drawStaticScene()
{
    if (!m_static)
    {
        std::vector<Point> pts;
        int count = 100;
        float xbegin = -static_cast<float>(kDefaultDesignSize / 2);
        float ybegin = -static_cast<float>(kDefaultDesignSize / 2);
        float xrange = static_cast<float>(kDefaultDesignSize);
        float yrange = static_cast<float>(kDefaultDesignSize);
        float ystride = xrange / count;
        float xstride = xrange / count;
        float tri_size = xstride * 0.8;
        for (int row = 0; row < count; row++)
        {
            float ypos = ybegin + row * ystride;
            for (int col = 0; col < count; col++)
            {
                float xpos = xbegin + col * xstride;
                Point pta(xpos, ypos);
                Point ptb(xpos + tri_size, ypos);
                Point ptc(xpos, ypos + tri_size);
                pts.emplace_back(pta);
                pts.emplace_back(ptb);
                pts.emplace_back(ptc);
            }
        }
        m_static = new GLObject2D(m_renderer, DrawMode::Fill);
        m_static->setColor(kColorGreen);
        m_static->setOpaque(0.5f);
        m_static->upload(pts.data(), int(pts.size()));
    }
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);
    m_renderer->paintObject(m_static);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FBOOperation::applyTexture()
{
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!m_tex_obj)
    {
        m_tex_obj = new GLObject2D(m_renderer, DrawMode::Fill);
    }
    std::vector<Point> buffer;
    const Box& view = m_renderer->viewPort()->getView();
    Point pt0(view.xmin, view.ymin);
    Point pt1(view.xmax, view.ymin);
    Point pt2(view.xmax, view.ymax);
    Point pt3(view.xmin, view.ymax);
    Point uv0(0.0f, 0.0f);
    Point uv1(1.0f, 0.0f);
    Point uv2(1.0f, 1.0f);
    Point uv3(0.0f, 1.0f);
    std::vector<Point> pts = { pt0, uv0, pt1, uv1, pt2, uv2, pt0, uv0, pt2,uv2, pt3, uv3 };
    m_tex_obj->upload(pts.data(), int(pts.size()));

    ProgramManager* program = m_renderer->programMgr();;
    ViewBox vb = Utils::toViewBox(m_renderer->viewPort()->getView());
    program->applyProgram(ProgramType::Texture2D);
    program->uniformViewBox(vb);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    m_tex_obj->drawAsFont();
    glBindTexture(GL_TEXTURE_2D, 0);
    program->releaseProgram();
}

void FBOOperation::drawDynamicScene()
{
    if (!m_drag)
    {
        return;
    }

    if (!m_dynamic)
    {
        m_dynamic = new GLObject2D(m_renderer, DrawMode::Fill);
        m_dynamic->setColor(kColorCyan);
        m_dynamic->setOpaque(0.5f);
    }
    float size = -static_cast<float>(kDefaultDesignSize / 10);
    Point pt1 = toFPoint(m_mouse_pt);
    Point pt2(pt1.x + size, pt1.y);
    Point pt3(pt1.x, pt1.y + size);
    std::vector<Point> pts{pt1, pt2, pt3};
    m_dynamic->upload(pts.data(), int(pts.size()));
    m_renderer->paintObject(m_dynamic);
}
