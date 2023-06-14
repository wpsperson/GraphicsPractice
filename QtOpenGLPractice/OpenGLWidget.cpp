#include "OpenGLWidget.h"
#include <iostream>
#include "Renderer.h"


OpenGLWidget::OpenGLWidget(QWidget* parent/* = 0*/)
{
    m_renderer = new Renderer();
    this->resize(1000, 1000);
}

OpenGLWidget::~OpenGLWidget()
{
    delete m_renderer;
}

void OpenGLWidget::initializeGL()
{
    std::string errorMessage;
    if (!m_renderer->initialize(errorMessage))
    {
        std::cout << errorMessage << std::endl;
    }
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