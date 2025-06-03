#include "GLObject2D.h"

#include <iostream>
#include <chrono>
#include <cassert>
#include <exception>
#include "OpenGLHeader.h"
#include "Renderer.h"
#include "ShaderSource.h"

static constexpr int TwoDim = 2;
static constexpr int Four = 4;


GLObject2D::GLObject2D(Renderer* renderer, DrawMode mode) : m_renderer(renderer), m_mode(mode)
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

void GLObject2D::setOpaque(float opaque) noexcept
{
    m_opaque = opaque;
}

float GLObject2D::opaque() const noexcept
{
    return m_opaque;
}

int GLObject2D::count() const noexcept
{
    return m_point_count;
}

void GLObject2D::upload(Point* buffer, int count)
{
    m_point_count = count;
    std::size_t size = count * sizeof(Point);
    if (m_vbo == 0)
    {
        glGenBuffers(1, &m_vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
}

void GLObject2D::draw()
{
    GLenum Primitive = (m_mode == DrawMode::Fill) ? GL_TRIANGLES : ((m_mode == DrawMode::Lines) ? GL_LINES : GL_POINTS);
    assert(m_vbo > 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, TwoDim, GL_FLOAT, GL_FALSE, TwoDim * sizeof(float), (void*)0);
    glDrawArrays(Primitive, 0, m_point_count);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLObject2D::drawAsFont()
{
    GLenum Primitive = (m_mode == DrawMode::Fill) ? GL_TRIANGLES : ((m_mode == DrawMode::Lines) ? GL_LINES : GL_POINTS);
    assert(m_vbo > 0);
    int text_pt_count = m_point_count / 2;
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, TwoDim, GL_FLOAT, GL_FALSE, Four * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, TwoDim, GL_FLOAT, GL_FALSE, Four * sizeof(float), (void*)(TwoDim *sizeof(float)));
    glDrawArrays(Primitive, 0, text_pt_count);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

