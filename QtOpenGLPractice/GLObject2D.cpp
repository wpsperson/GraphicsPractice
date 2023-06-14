#include "GLObject2D.h"

#include <iostream>
#include <chrono>
#include <cassert>
#include <exception>
#include "glad/glad.h"
#include "Renderer.h"
#include "ShaderSource.h"

static constexpr int TwoDim = 2;


GLObject2D::GLObject2D(Renderer* renderer) : m_renderer(renderer)
{

}

GLObject2D::~GLObject2D()
{
    glDeleteBuffers(1, &m_vbo);
}

void GLObject2D::setColor(const Color3f& color) noexcept
{
    m_color = color;
}

const Color3f& GLObject2D::color() const noexcept
{
    return m_color;
}

void GLObject2D::setOpaque(unsigned char opaque) noexcept
{
    m_opaque = opaque;
}

unsigned char GLObject2D::opaque() const noexcept
{
    return m_opaque;
}

void GLObject2D::upload(Point* buffer, int count)
{
    m_point_count = count;
    std::size_t size = count * sizeof(Point);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
}

void GLObject2D::draw()
{
    assert(m_vbo > 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, TwoDim, GL_FLOAT, GL_FALSE, TwoDim * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, m_point_count);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

