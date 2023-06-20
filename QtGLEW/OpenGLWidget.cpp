#include "OpenGLWidget.h"
#include "Renderer.h"


OpenGLWidget::OpenGLWidget(QWidget* parent/* = 0*/)
{
    m_renderer = new Renderer();
}

OpenGLWidget::~OpenGLWidget()
{
    delete m_renderer;
}

void OpenGLWidget::initializeGL()
{
    m_renderer->init();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    m_width = w;
    m_height = h;
    m_renderer->resize(w, h);
}

void OpenGLWidget::paintGL()
{
    m_renderer->render();
}