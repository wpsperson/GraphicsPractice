#pragma once

#include "Operations/Operation.h"
#include "Core/Mesh.h"
#include "Util/ManyCircles.h"
#include "Util/ManyRectangles.h"
#include "Components/PersistMapRingBuffer.h"
#include "Components/StippleManager.h"

class StippleOperation : public Operation
{
public:
    StippleOperation();

    ~StippleOperation();

    void initialize(Renderer* renderer) noexcept override;

    void resizeOperation(int width, int height) noexcept override;

    void paint(Renderer* renderer) noexcept override;

private:
    PersistMapRingBuffer* m_ringbuffer = nullptr;
    PersistMapStipple* m_ringbuffer2 = nullptr;
    StippleManager* m_stip_mgr = nullptr;
    ManyCircles m_circles;
    ManyRectangles m_rects;
    ColorMesh m_tempMesh;
    ColorStipMesh m_tempMesh2;
    glint64 hrange = 10000; 
    Renderer* m_renderer = nullptr;
};