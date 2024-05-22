#include "OpenGLWidget.h"
#include "Renderer.h"

#include <QtWidgets/QProgressDialog>
#include <QtCore/QCoreApplication>

#include <Windows.h>

OpenGLWidget::OpenGLWidget(QWidget* parent/* = 0*/)
{
    m_renderer = new Renderer();
}

OpenGLWidget::~OpenGLWidget()
{
    delete m_renderer;
}

void OpenGLWidget::setPopup(bool flag)
{
    m_popup = flag;
}

void OpenGLWidget::initializeGL()
{
    if (!m_popup)
    {
        m_renderer->init();
        return;
    }

    //QProgressDialog *progress = new QProgressDialog(this);
    //progress->setRange(0, 100);
    //progress->setModal(true);
    //progress->show();
    //progress->setValue(20);
    //Sleep(500);
    //QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    //progress->setValue(40);
    //Sleep(500);
    //QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    //progress->setValue(60);
    //Sleep(500);
    //QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    //progress->accept();

    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

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