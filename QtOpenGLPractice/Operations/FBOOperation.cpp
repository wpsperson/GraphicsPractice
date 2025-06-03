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
}

void FBOOperation::initialize(Renderer* renderer) noexcept
{
    m_renderer = renderer;





}

void FBOOperation::paint(Renderer* renderer) noexcept
{
    // try to draw a texture.

    if (0 == m_texture)
    {
        std::vector<unsigned char> images;
        images.reserve(m_width * m_height * 4);
        for (int idx = 0; idx < m_height; idx++)
        {
            unsigned char blue = (idx * 255) / m_height;
            for (int col = 0; col < m_width; col++)
            {
                images.emplace_back(uchar(127));
                images.emplace_back(uchar(127));
                images.emplace_back(uchar(blue));
                images.emplace_back(uchar(255));
            }
        }
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, images.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


        std::vector<Point> buffer;
        // buffer emplace back both point and uvs;

        const Box& view = m_renderer->viewPort()->getView();
        //Point pt0(view.xmin, view.ymin);
        //Point pt1(view.xmax, view.ymin);
        //Point pt2(view.xmax, view.ymax);
        //Point pt3(view.xmin, view.ymax);
        Point pt0(0.0f, 0.0f);
        Point pt1(100000.0f, 0.0f);
        Point pt2(100000.0f, 100000.0f);
        Point pt3(0.0f, 100000.0f);
        Point uv0(0.0f, 0.0f);
        Point uv1(1.0f, 0.0f);
        Point uv2(1.0f, 1.0f);
        Point uv3(0.0f, 1.0f);
        std::vector<Point> pts = { pt0, uv0, pt1, uv1, pt2, uv2, pt0, uv0, pt2,uv2, pt3, uv3 };
        m_tex_obj = new GLObject2D(m_renderer, DrawMode::Fill);
        m_tex_obj->upload(pts.data(), pts.size());
    }

    ProgramManager* program = m_renderer->programMgr();;
    ViewBox vb = Utils::toViewBox(m_renderer->viewPort()->getView());
    program->applyProgram(ProgramType::Texture2D);
    program->uniformViewBox(vb);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    m_tex_obj->drawAsFont();
    glBindTexture(GL_TEXTURE_2D, 0);
    program->releaseProgram();


    return;
    if (m_support_fbo)
    {
        // first time
        if (!m_fbo_finish)
        {
            if (initFBO())
            {
                drawStaticScene();
                m_fbo_finish = true;
            }
        }
        // loop
        if (m_fbo_finish)
        {
            applyTexture();
            //drawStaticScene();
            //drawDynamicScene();
        }
    }
    else
    {

    }
}

void FBOOperation::processMouseClick(QMouseEvent* eve)
{
    if (eve->button() == Qt::LeftButton)
    {
        m_drag = true;
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
        m_static->upload(pts.data(), pts.size());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
    //const Box& view = m_renderer->viewPort()->getView();
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glOrtho(view.xmin, view.xmax, view.ymin, view.ymax, -1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    m_renderer->paintObject(m_static);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBOOperation::applyTexture()
{
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    if (!m_tex_obj)
    {
        const Box& view = m_renderer->viewPort()->getView();
        Point pta(view.xmin, view.ymin);
        Point ptb(view.xmax, view.ymin);
        Point ptc(view.xmax, view.ymax);
        Point ptd(view.xmin, view.ymax);
        std::vector<Point> pts{ pta, ptb, ptc, ptc, ptd, pta };
        m_static = new GLObject2D(m_renderer, DrawMode::Fill);
        m_static->setColor(kColorGreen);
        m_static->setOpaque(0.5f);
        m_static->upload(pts.data(), pts.size());
    }

    ProgramManager* program = m_renderer->programMgr();
    program;

    glBindTexture(GL_TEXTURE_2D, 0);
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
    m_dynamic->upload(pts.data(), pts.size());
    m_renderer->paintObject(m_dynamic);
}
