#pragma once

#include "Operations/Operation.h"

#include <vector>

class CirclesColorUniform : public Operation
{
public:
    ~CirclesColorUniform();

    void initialize(Renderer* renderer) noexcept override;

    void paint(Renderer* renderer) noexcept override;

};