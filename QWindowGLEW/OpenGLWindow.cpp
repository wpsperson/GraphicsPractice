#include "openglwindow.h"


#define GLEW_STATIC
#include <GL/glew.h>

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QDebug>
#include <QCoreApplication>



OpenGLWindow::OpenGLWindow(QWindow* parent)
    : QWindow(parent)
    , m_context(nullptr)
    , m_update_pending(false)
{
    setSurfaceType(QWindow::OpenGLSurface);

    // 设置OpenGL版本 (使用OpenGL ES 2.0，兼容性更好)
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2, 1);
    setFormat(format);
}

OpenGLWindow::~OpenGLWindow()
{
    delete m_context;
}

void OpenGLWindow::render()
{
    if (!isExposed())
        return;

    // 初始化OpenGL上下文
    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();
        m_context->makeCurrent(this);
        // init
        GLenum GlewInitResult = glewInit();
        if (GLEW_OK != GlewInitResult)
        {
            const char* error_message = reinterpret_cast<const char*>(glewGetErrorString(GlewInitResult));
            if (error_message)
            {
                qDebug() << error_message;
            }
            exit(EXIT_FAILURE);
        }
        qDebug() << "OpenGL initialized:" << (const char*)glGetString(GL_VERSION);
    }
    // 初始化（只执行一次）
    static bool initialized = false;
    if (!initialized) {
        initialize();
        initialized = true;
    }

    m_context->makeCurrent(this);
    // 设置视口
    glViewport(0, 0, width(), height());
    // 设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);  // 2D正交投影
        // 设置模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 清除颜色缓冲区
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 在这里添加你的OpenGL绘制代码
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);  // 红色
    glVertex3f(-0.5f, -0.5f, 0.0f);  // 左下角
    glColor3f(0.0f, 1.0f, 0.0f);  // 绿色
    glVertex3f(0.5f, -0.5f, 0.0f);   // 右下角
    glColor3f(0.0f, 0.0f, 1.0f);  // 蓝色
    glVertex3f(0.0f, 0.5f, 0.0f);    // 顶部
    glEnd();

    m_context->swapBuffers(this);
}

void OpenGLWindow::initialize()
{
    // OpenGL初始化代码可以放在这里
    // 例如编译着色器、加载纹理等
}

void OpenGLWindow::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void OpenGLWindow::renderNow()
{
    m_update_pending = false;
    render();
}

bool OpenGLWindow::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent* event)
{
    Q_UNUSED(event);

    if (isExposed()) {
        renderNow();
    }
}

void OpenGLWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);

    if (isExposed()) {
        renderLater();
    }
}