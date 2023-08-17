#include "Operation.h"


void Operation::setName(const std::string& name) noexcept
{
    m_name = name;
}

const std::string& Operation::name() const noexcept
{
    return m_name;
}

void Operation::setWidget(OpenGLWidget* widget)
{
    m_widget = widget;
}

void Operation::initialize(Renderer* renderer) noexcept
{

}

void Operation::paint(Renderer* renderer) noexcept
{
}

void Operation::processMouseClick(QMouseEvent* eve)
{
}

void Operation::processMouseRelease(QMouseEvent* eve)
{
}

void Operation::processDoubleMouseClick(QMouseEvent* eve)
{
}

void Operation::processMouseMove(QMouseEvent* eve)
{
}

void Operation::processKeyPress(QKeyEvent* event)
{
}
