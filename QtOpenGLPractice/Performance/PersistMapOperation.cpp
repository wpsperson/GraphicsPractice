#include "Performance/PersistMapOperation.h"

#include <iostream>

#include "Components/Renderer.h"
#include "Components/ViewPort.h"
#include "Components/ProgramManager.h"
#include "Util/Utils.h"
#include "Util/ArgumentUtil.h"


PersistMapOperation::PersistMapOperation()
{
    // arguments:  PersistMapOperation  CirNumDim=100 PersistMem=256
    int value = 0;
    if (ArgumentUtil::getValueByKey("CirNumDim", value))
    {
        m_circles.setCircleNumDim(value);
    }
    if (ArgumentUtil::getValueByKey("PersistMem", value))
    {
        MemorySegment::setSegmentCapacityKB(value);
    }

}

PersistMapOperation::~PersistMapOperation()
{
    delete m_ringbuffer;
}

void PersistMapOperation::initialize(Renderer* renderer) noexcept
{
    glint64 hrange = 10000;
    renderer->viewPort()->setDesign(-hrange, -hrange, 2 * hrange, 2 * hrange);
    m_ringbuffer = new PersistMapRingBuffer;
    m_ringbuffer->initBuffer();
    m_circles.setRange(float(hrange));
    m_circles.setCircleNumInBatch(10);
}

void PersistMapOperation::paint(Renderer* renderer) noexcept
{
    m_circles.rebuildRandomInfos();

    ProgramManager*progMgr = renderer->programMgr();
    progMgr->applyProgram(ProgramType::ColorVertex);
    ViewBox viewbox = Utils::toViewBox(renderer->viewPort()->getView());
    progMgr->uniformViewBox(viewbox);
    m_ringbuffer->bindVAO();

    int batch_count = m_circles.batchCount();
    for (int idx = 0; idx < batch_count; idx++)
    {
        m_tempMesh.resetMesh();
        m_circles.buildBatchFillMesh(idx, m_tempMesh);
        m_ringbuffer->processMesh(m_tempMesh);
    }
    m_ringbuffer->drawCurrentSegmentBuffer();
}
