#include "OpenGLWidget.h"
#include <iostream>

#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QFileDialog>

#include "Components/Renderer.h"
#include "Components/ViewPort.h"
#include "Operations/Operation.h"

Operation* createDefaultOperation(const std::string& name);

OpenGLWidget::OpenGLWidget(QWidget* parent/* = 0*/)
{
    m_renderer = new Renderer();
    this->resize(1000, 1000);
    setMouseTracking(true); // mouse move
    setFocusPolicy(Qt::StrongFocus); // key input
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

    std::string name = this->objectName().toStdString();
    m_operation = createDefaultOperation(name);
    m_operation->setWidget(this);
    m_operation->initialize(m_renderer);
}

void OpenGLWidget::resizeGL(int width, int height)
{
    m_width = width;
    m_height = height;
    m_renderer->resize(width, height);
    m_operation->resizeOperation(width, height);
}

void OpenGLWidget::paintGL()
{
    m_renderer->beforeRender();
    m_operation->paint(m_renderer);
    m_renderer->endRender();
    update();

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
    m_operation->processMouseWheel(eve);
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
    m_operation->processKeyPress(event);
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    QOpenGLWidget::mousePressEvent(event);
    m_operation->processMouseClick(event);
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QOpenGLWidget::mouseReleaseEvent(event);
    m_operation->processMouseRelease(event);
}

void OpenGLWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    QOpenGLWidget::mouseDoubleClickEvent(event);
    m_operation->processDoubleMouseClick(event);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    QOpenGLWidget::mouseMoveEvent(event);
    m_operation->processMouseMove(event);
}
