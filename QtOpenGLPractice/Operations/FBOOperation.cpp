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
#include "Geometry.h"

FBOOperation::FBOOperation()
{
}

FBOOperation::~FBOOperation()
{
    if (m_sten_void)
    {
        delete m_sten_void;
    }
    if (m_sten_obj)
    {
        delete m_sten_obj;
    }
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
    if (m_stencil)
    {
        glDeleteRenderbuffers(1, &m_stencil);
    }
}

void FBOOperation::initialize(Renderer* renderer) noexcept
{
    m_renderer = renderer;
    std::pair<int, int> version = m_renderer->getVersion();
    if (version.first >= 3)
    {
        m_support_fbo = true;
    }
    initFBO();
}

void FBOOperation::paint(Renderer* renderer) noexcept
{
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

void FBOOperation::resizeOperation(int width, int height) noexcept
{
    Operation::resizeOperation(width, height);
    double ratio = m_widget->devicePixelRatioF();
    m_device_width = m_width * ratio;
    m_device_height = m_height * ratio;

    // resize texture.
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_device_width, m_device_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, m_stencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_device_width, m_device_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    std::cout << "resize event...." << std::endl;
    m_texture_done = false;
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
    GLint origin_fbo = 0;
    if (m_support_fbo)
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &origin_fbo);
        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    }

    // no matter GPU support fbo, we use texture.
    int dummy = 1000;
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, dummy, dummy, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (m_support_fbo)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

        glGenRenderbuffers(1, &m_stencil);
        glBindRenderbuffer(GL_RENDERBUFFER, m_stencil);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dummy, dummy);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_stencil);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencil);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Fail to complete frame buffer!" << std::endl;
            return false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, origin_fbo);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
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


        std::vector<Point> circle1 = generateCircle(0.0f, 0.0f, 100000.0f, 32);
        std::vector<Point> circle2 = generateCircle(100000.0f, 0.0f, 100000.0f, 32);
        std::vector<Point> circle3 = generateCircle(200000.0f, 0.0f, 100000.0f, 32);
        pts.clear();
        std::copy(circle1.begin(), circle1.end(), std::back_inserter(pts));
        std::copy(circle2.begin(), circle2.end(), std::back_inserter(pts));
        std::copy(circle3.begin(), circle3.end(), std::back_inserter(pts));
        m_sten_obj = new GLObject2D(m_renderer, DrawMode::Fill);
        m_sten_obj->setColor(kColorBlue);
        m_sten_obj->setOpaque(0.5f);
        m_sten_obj->upload(pts.data(), int(pts.size()));

        std::vector<Point> void_pts = generateCircle(0.0, 0.0f, 50000.0f, 32);
        m_sten_void = new GLObject2D(m_renderer, DrawMode::Fill);
        m_sten_void->upload(void_pts.data(), int(void_pts.size()));
    }

    GLint origin_fbo = 0;
    if (m_support_fbo)
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &origin_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    }

    glViewport(0, 0, m_device_width, m_device_height);
    glClear(GL_COLOR_BUFFER_BIT);
    m_renderer->paintObject(m_static);
    // draw a stencil object
    m_renderer->beginStencil();
    m_renderer->stencilVoidBegin();
    m_renderer->paintObject(m_sten_void);
    m_renderer->stencilVoidEnd();
    m_renderer->stencilFill();
    m_renderer->paintObject(m_sten_obj);
    m_renderer->endStencil();

    if (m_support_fbo)
    {
        // restore to origin fbo in QOpenGLWidget
        glBindFramebuffer(GL_FRAMEBUFFER, origin_fbo);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, m_device_width, m_device_height, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

void FBOOperation::applyTexture()
{
    glViewport(0, 0, m_device_width, m_device_height);
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
