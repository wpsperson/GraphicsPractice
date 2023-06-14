#pragma once 

#include "Operation.h"
#include "GLObject2D.h"

void injectAllOperation(Renderer* renderer);

class FirstTriangle : public Operation
{
public:
    ~FirstTriangle();

    virtual void initialize(Renderer* renderer) noexcept;

    virtual void paint(Renderer* renderer) noexcept;

private:
    GLObject2D* m_object = nullptr;
};

