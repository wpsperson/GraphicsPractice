#pragma once

#include "Operations/Operation.h"
#include "Model/Mesh.h"
#include "Model/ManyCircles.h"
#include "Model/PersistMapRingBuffer.h"
#include "Components/StippleManager.h"

class StippleOperation : public Operation
{
public:
    StippleOperation();

    ~StippleOperation();

    void initialize(Renderer* renderer) noexcept override;

    void paint(Renderer* renderer) noexcept override;

private:
    PersistMapRingBuffer* m_ringbuffer = nullptr;
    StippleManager* m_stip_mgr = nullptr;
    ManyCircles m_circles;
    ColorMesh m_tempMesh;
};