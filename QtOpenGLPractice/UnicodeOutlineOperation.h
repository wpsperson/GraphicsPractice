#pragma once

#include <vector>

#include "Operation.h"
#include "GLObject2D.h"
#include "Utils.h"
#include "GlyphOutlineGenerator.h"

class UnicodeOutlineOperation : public Operation
{
public:
    ~UnicodeOutlineOperation();

    virtual void initialize(Renderer* renderer) noexcept;

    virtual void paint(Renderer* renderer) noexcept;

private:
    GLObject2D* m_object = nullptr;
    GlyphOutlines m_outln;
    GlyphOutlineGenerator m_generator;
    std::vector<Point> m_mesh;
    bool m_init_success = false;
};