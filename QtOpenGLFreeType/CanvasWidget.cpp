#include "CanvasWidget.h"
#include <QEvent>
#include <QMouseEvent>

#include <GL/GL.h>
#include <GL/GLU.h>

#include "freetype/freetype.h"

static int Width = 300;
static int Height = 300;

CanvasWidget::CanvasWidget()
{
    FT_Library ft;
    FT_Error ret = FT_Init_FreeType(&ft);
}

CanvasWidget::~CanvasWidget()
{

}

void CanvasWidget::initializeGL()
{
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
}

void CanvasWidget::resizeGL(int w, int h)
{
    m_width = w;
    m_height = h;
    glViewport(0, 0, w, h);
    gluOrtho2D(0, w, 0, h);
}

void CanvasWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(0.5f, 0.5f, 0.8f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(m_x0, m_y0);
    glVertex2f(m_x1, m_y0);
    glVertex2f(m_x1, m_y1);
    glVertex2f(m_x0, m_y1);
    glEnd();
}

void CanvasWidget::mousePressEvent(QMouseEvent* event)
{
    m_picked++;
    if (m_picked == 1)
    {
        m_x0 = m_x1 = event->x();
        m_y0 = m_y1 = m_height - event->y();
    }
    else if (m_picked == 2)
    {
        m_picked = 0;
    }
    repaint();

    QWidget::mousePressEvent(event);
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QWidget::mouseReleaseEvent(event);
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_picked == 1)
    {
        m_x1 = event->x();
        m_y1 = m_height - event->y();
        repaint();
    }
    QWidget::mouseMoveEvent(event);
}
