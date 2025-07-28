#include "Performance/PersistMapOperation.h"

#include <iostream>

#include "Renderer.h"
#include "ProgramManager.h"
#include "Util/ArgumentUtil.h"


PersistMapOperation::PersistMapOperation()
{
    // arguments:  PersistMapOperation  CirNumDim=100 PersistMem=256
    int value = 0;
    if (ArgumentUtil::getValueByKey("CirNumDim", value))
    {
        ManyCircles::setCircleNumDim(value);
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
    m_ringbuffer = new PersistMapRingBuffer;
    m_ringbuffer->initBuffer();
    m_circles.setCircleNumInBatch(10);
}

void PersistMapOperation::paint(Renderer* renderer) noexcept
{
    m_circles.rebuildRandomInfos();

    ProgramManager*progMgr = renderer->programMgr();
    progMgr->applyProgram(ProgramType::ColorVertex);
    m_ringbuffer->bindVAO();

    int batch_count = m_circles.batchCount();
    for (int idx = 0; idx < batch_count; idx++)
    {
        m_tempMesh.resetMesh();
        m_circles.buildBatchMesh(idx, m_tempMesh);
        m_ringbuffer->processMesh(m_tempMesh);
    }
    m_ringbuffer->drawCurrentSegmentBuffer();
}
