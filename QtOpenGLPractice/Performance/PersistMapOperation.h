#pragma once

#include "Operations/Operation.h"

#include <array>

#include "OpenGLHeader.h"
#include "Const.h"
#include "Util/ManyCircles.h"
#include "Core/Mesh.h"
#include "Components/SegmentMemory.h"
#include "Components/PersistMapRingBuffer.h"


class PersistMapOperation : public Operation
{
public:
    PersistMapOperation();

    ~PersistMapOperation();

    void initialize(Renderer* renderer) noexcept override;

    void paint(Renderer* renderer) noexcept override;

private:
    PersistMapRingBuffer* m_ringbuffer = nullptr;
    ManyCircles m_circles;
    ColorMesh m_tempMesh;
};




