#include "DemoOperation.h"
#include "Renderer.h"
#include "ProgramManager.h"
#include "FontManager.h"
#include "glad/glad.h"

void injectAllOperation(Renderer* renderer)
{
    FirstTriangle* ft = new FirstTriangle;
    ft->setName("FirstTriangle");
    renderer->addOperation(ft);
    FontOperation* font = new FontOperation;
    font->setName("FontOperation");
    renderer->addOperation(font);
    MillionPrimitiveOperation* million = new MillionPrimitiveOperation;
    million->setName("MillionPrimitiveOperation");
    renderer->addOperation(million);

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

MillionPrimitiveOperation::~MillionPrimitiveOperation()
{
}

void MillionPrimitiveOperation::initialize(Renderer* renderer) noexcept
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_width, 0, m_height, -1, 1);
}

void MillionPrimitiveOperation::paint(Renderer* renderer) noexcept
{
    float advance = m_width / m_repeat_count;

    Point triangle[3];
    triangle[0].x = 0.0, triangle[0].y = 0;
    triangle[1].x = advance, triangle[1].y = 0;
    triangle[2].x = advance / 2, triangle[2].y = advance;
    Point pt0{ 0.0, advance / 2 };
    Point pt1{ advance, advance / 2 };

    float xOffset = 0, yOffset = 0, zOffset = 0;
    float xStep = advance;
    float yStep = advance;

    // batch strategy spend average 100ms, while retail strategy spend average 400ms
    // same-state batch draw vs one by one draw:
    // glBegin draw mode interleave: Triangles/Lines 100ms : 400ms
    // glColor interleave: 150ms : 200 ms
    // enable/disable line stipple 150ms : 1000ms
    // enable line stipple, solid line/phantom line: 100ms/1000ms

    glEnable(GL_LINE_STIPPLE);
    if (m_batch_strategy)
    {
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < m_repeat_count; i++)
        {
            xOffset = i * xStep;
            for (int j = 0; j < m_repeat_count; j++)
            {
                yOffset = j * yStep;
                glVertex2f(triangle[0].x + xOffset, triangle[0].y + yOffset);
                glVertex2f(triangle[1].x + xOffset, triangle[1].y + yOffset);
                glVertex2f(triangle[2].x + xOffset, triangle[2].y + yOffset);
            }
        }
        glEnd();
    }
    else
    {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < m_repeat_count; i++)
        {
            xOffset = i * xStep;
            glColor3f(i * 0.001f, 0.5f, 0.5f);
            for (int j = 0; j < m_repeat_count; j++)
            {
                yOffset = j * yStep;
                glVertex2f(triangle[0].x + xOffset, triangle[0].y + yOffset);
                glVertex2f(triangle[1].x + xOffset, triangle[1].y + yOffset);
                glVertex2f(triangle[2].x + xOffset, triangle[2].y + yOffset);
            }
        }
        glEnd();
    }
}
