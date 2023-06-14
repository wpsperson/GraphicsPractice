#include "DemoOperation.h"
#include "Renderer.h"
#include "ProgramManager.h"
#include "FontManager.h"

void injectAllOperation(Renderer* renderer)
{
    FirstTriangle* ft = new FirstTriangle;
    ft->setName("FirstTriangle");
    renderer->addOperation(ft);
    FontOperation* font = new FontOperation;
    font->setName("FontOperation");
    renderer->addOperation(font);
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
    m_object->setOpaque(0.5f);
    m_object->upload(pts.data(), 3);
}

void FirstTriangle::paint(Renderer* renderer) noexcept
{
    renderer->paintObject(m_object);
}

FontOperation::~FontOperation()
{
    if (m_object)
    {
        delete m_object;
    }
}

void FontOperation::initialize(Renderer* renderer) noexcept
{
    FontManager* fontMgr = renderer->fontMgr();
    const AsciiCharInfo *info = fontMgr->asciiChar('A');

    Point pt0(-0.5f, -0.5f);
    Point pt1(0.5f, -0.5f);
    Point pt2(0.5f, 0.5f);
    Point pt3(-0.5f, 0.5f);
    Point uv0(info->uv_min_x, info->uv_min_y);
    Point uv1(info->uv_max_x, info->uv_min_y);
    Point uv2(info->uv_max_x, info->uv_max_y);
    Point uv3(info->uv_min_x, info->uv_max_y);
    //Point uv0(0.0f, 0.0f);
    //Point uv1(0.8f, 0.0f);
    //Point uv2(0.8f, 0.8f);
    //Point uv3(0.0f, 0.8f);
    std::vector<Point> pts = { pt0, uv0, pt1, uv1, pt2, uv2, pt0, uv0, pt2,uv2, pt3, uv3 };
    m_object = new GLObject2D(renderer);
    m_object->setColor(kColorGreen);
    m_object->setOpaque(1.0f);
    m_object->upload(pts.data(), 12);
}

void FontOperation::paint(Renderer* renderer) noexcept
{
    renderer->paintFont(m_object);
}
