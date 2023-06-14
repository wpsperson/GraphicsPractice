#pragma once

#include <vector>
#include "Const.h"

class Renderer;
// 2d mesh float mesh
class GLObject2D
{
public:
    GLObject2D(Renderer *renderer);
    ~GLObject2D();

    void setColor(const Color3f& color) noexcept;
    const Color3f& color() const noexcept;

    void setOpaque(unsigned char opaque) noexcept;
    unsigned char opaque() const noexcept;

    void upload(Point *buffer, int count);

    void draw();

private:
    Renderer* m_renderer = nullptr;
    unsigned int m_vbo = 0;
    int m_point_count = 0;
    Color3f m_color = kColorBlue;
    unsigned char m_opaque = 255;
};