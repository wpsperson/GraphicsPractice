#pragma once

#include <string>

class Renderer;
class QMouseEvent;
class QKeyEvent;
class OpenGLWidget;
class QWheelEvent;

class Operation
{
public:
    virtual ~Operation() = default;

    void setName(const std::string& name) noexcept;

    const std::string& name() const noexcept;

    void setWidget(OpenGLWidget* widget);

    virtual void initialize(Renderer *renderer) noexcept;

    virtual void paint(Renderer* renderer) noexcept;

    virtual void resizeOperation(int width, int height) noexcept;

    virtual void processMouseClick(QMouseEvent* eve);

    virtual void processMouseRelease(QMouseEvent* eve);

    virtual void processDoubleMouseClick(QMouseEvent* eve);

    virtual void processMouseMove(QMouseEvent* eve);

    virtual void processKeyPress(QKeyEvent* event);

    virtual void processMouseWheel(QWheelEvent* eve);

protected:
    std::string m_name;
    OpenGLWidget* m_widget = nullptr;
    int m_width = 0;
    int m_height = 0;
};
