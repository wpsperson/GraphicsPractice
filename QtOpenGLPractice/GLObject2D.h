#pragma once

#include <vector>
#include "Const.h"

class Renderer;
// 2d mesh float mesh
class GLObject2D
{
public:
    GLObject2D(Renderer *renderer, DrawMode mode);
    ~GLObject2D();

    void setColor(const Color3f& color) noexcept;
    const Color3f& color() const noexcept;

    void setOpaque(float opaque) noexcept;
    float opaque() const noexcept;

    int count() const noexcept;

    void upload(Point *buffer, int count);

    void draw();

    void drawAsFont();

private:
    Renderer* m_renderer = nullptr;
    unsigned int m_vbo = 0;
    int m_point_count = 0;
    Color3f m_color = kColorBlue;
    float m_opaque = 1.0f;
    DrawMode m_mode;
};