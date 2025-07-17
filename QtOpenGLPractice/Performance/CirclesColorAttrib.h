#pragma once

#include "Operations/Operation.h"

#include <vector>

class CirclesColorAttrib : public Operation
{
public:
    ~CirclesColorAttrib();

    void initialize(Renderer* renderer) noexcept override;

    void paint(Renderer* renderer) noexcept override;

};