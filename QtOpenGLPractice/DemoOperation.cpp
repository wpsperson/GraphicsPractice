#include "DemoOperation.h"
#include "Renderer.h"
#include "programmanager.h"

void injectAllOperation(Renderer* renderer)
{
    FirstTriangle* ft = new FirstTriangle;
    ft->setName("FirstTriangle");
    renderer->addOperation(ft);
}

FirstTriangle::~FirstTriangle()
{
    if (m_object)
    {
        delete m_object;
    }
}

void FirstTriangle::initialize(Renderer* renderer) noexcept
{
    std::vector<Point> pts = { Point(-0.5f, -0.5f), Point(0.5f, -0.5f), Point(0.5f, 0.5f) };
    m_object = new GLObject2D(renderer);
    m_object->setColor(kColorGreen);
    m_object->setOpaque(127);
    m_object->upload(pts.data(), 3);
}

void FirstTriangle::paint(Renderer* renderer) noexcept
{
    renderer->paintObject(m_object);
}
