#pragma once

#include "Operations/Operation.h"
#include "OpenGLHeader.h"
#include "Const.h"

class PersistMapOperation : public Operation
{
public:
    PersistMapOperation();

    ~PersistMapOperation();

    void initialize(Renderer* renderer) noexcept override;

    void paint(Renderer* renderer) noexcept override;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    Point *m_persistBuffer = nullptr;
};




