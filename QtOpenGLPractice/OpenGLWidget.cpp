#include "OpenGLWidget.h"
#include <iostream>

#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QFileDialog>

#include "Renderer.h"
#include "ViewPort.h"
#include "DemoOperation.h"


OpenGLWidget::OpenGLWidget(QWidget* parent/* = 0*/)
{
    m_renderer = new Renderer();
    this->resize(2000, 2000);
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
    m_oper = dynamic_cast<EditVectorFont*>(m_renderer->m_operation);
    m_oper->setWidget(this);
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

void OpenGLWidget::wheelEvent(QWheelEvent* eve)
{
    QOpenGLWidget::wheelEvent(eve);
    int x = eve->position().x();
    int y = eve->position().y();
    int delta = eve->angleDelta().y();
    llPoint fixed_pt = m_renderer->viewPort()->screenToDB(x, y);
    double zoom_factor = delta > 0 ? 0.8 : 1.25; // 0.5 : 2.0;
    m_renderer->viewPort()->zoomView(fixed_pt.x, fixed_pt.y, zoom_factor);
    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    glint64 delta_x = 0;
    glint64 delta_y = 0;
    glint64 width = m_renderer->viewPort()->getView().width();
    glint64 height = m_renderer->viewPort()->getView().height();
    glint64 delta = std::max(width / 10, height / 10);

    int key = event->key();
    if (Qt::Key_Up == key)
    {
        delta_y = delta;
    }else if (Qt::Key_Down == key)
    {
        delta_y = -delta;
    }
    else if (Qt::Key_Left == key)
    {
        delta_x = -delta;
    }
    else if (Qt::Key_Right == key)
    {
        delta_x = delta;
    }

    if (delta_x || delta_y)
    {
        m_renderer->viewPort()->moveView(delta_x, delta_y);
        update();
    }

    m_oper->processKeyPress(event);
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    QOpenGLWidget::mousePressEvent(event);
    m_oper->processMouseClick(event);
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QOpenGLWidget::mouseReleaseEvent(event);
    m_oper->processMouseRelease(event);
}

void OpenGLWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    QOpenGLWidget::mouseDoubleClickEvent(event);
    m_oper->processDoubleMouseClick(event);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    QOpenGLWidget::mouseMoveEvent(event);
    m_oper->processMouseMove(event);
}
