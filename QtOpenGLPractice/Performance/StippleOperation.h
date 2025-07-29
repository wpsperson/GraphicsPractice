#pragma once

#include "Operations/Operation.h"
#include "Model/Mesh.h"
#include "Util/ManyCircles.h"
#include "Components/PersistMapRingBuffer.h"
#include "Components/StippleManager.h"

class StippleOperation : public Operation
{
public:
    StippleOperation();

    ~StippleOperation();

    void initialize(Renderer* renderer) noexcept override;

    void paint(Renderer* renderer) noexcept override;

    void buildRectangles(glint64 half_range, ColorMesh &mesh);

private:
    PersistMapRingBuffer* m_ringbuffer = nullptr;
    StippleManager* m_stip_mgr = nullptr;
    ManyCircles m_circles;
    ColorMesh m_tempMesh;
    glint64 hrange = 10000;
};