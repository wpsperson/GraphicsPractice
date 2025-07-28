#pragma once

#include "Operations/Operation.h"

#include <vector>

#include "GLObject2D.h"
#include "Util/Utils.h"


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

class FontOperation : public Operation
{
public:
    ~FontOperation();

    virtual void initialize(Renderer* renderer) noexcept;

    virtual void paint(Renderer* renderer) noexcept;

private:
    GLObject2D* m_object = nullptr;
};

class MillionPrimitiveOperation : public Operation
{
public:
    ~MillionPrimitiveOperation();

    virtual void initialize(Renderer* renderer) noexcept;

    virtual void paint(Renderer* renderer) noexcept;

private:
    GLObject2D* m_object = nullptr;
    double m_width = 1000.0;
    double m_height = 1000.0;
    int m_repeat_count = 1000;
    bool m_batch_strategy = true;
};

