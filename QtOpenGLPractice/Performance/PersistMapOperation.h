#pragma once

#include "Operations/Operation.h"

#include <array>

#include "OpenGLHeader.h"
#include "Const.h"
#include "Model/ManyCircles.h"
#include "Model/Mesh.h"
#include "Model/SegmentMemory.h"
#include "Model/PersistMapRingBuffer.h"


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




