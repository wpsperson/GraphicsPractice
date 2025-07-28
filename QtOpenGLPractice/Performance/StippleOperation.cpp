#include "Performance/StippleOperation.h"


#include "OpenGLHeader.h"
#include "Components/Renderer.h"
#include "Components/ViewPort.h"
#include "Components/ProgramManager.h"
#include "Util/Utils.h"

StippleOperation::StippleOperation()
{

}

StippleOperation::~StippleOperation()
{
    delete m_ringbuffer;
}

void StippleOperation::initialize(Renderer* renderer) noexcept
{
    glint64 hrange = 10000;
    renderer->viewPort()->setDesign(-hrange, -hrange, 2 * hrange, 2 * hrange);

    m_ringbuffer = new PersistMapRingBuffer;
    m_ringbuffer->initBuffer();
    m_stip_mgr = new StippleManager;
    m_stip_mgr->initStippleManger();

    m_circles.setRange(float(hrange));
    m_circles.setCircleNumInBatch(10);
}

void StippleOperation::paint(Renderer* renderer) noexcept
{
    m_circles.rebuildRandomInfos();

    ProgramManager* progMgr = renderer->programMgr();
    progMgr->applyProgram(ProgramType::PolygonStipple);
    ViewBox viewbox = Utils::toViewBox(renderer->viewPort()->getView());
    progMgr->uniformViewBox(viewbox);
    m_ringbuffer->bindVAO();
    m_stip_mgr->setPolygonStipple(PolyStipple::PointHatch);

    int batch_count = m_circles.batchCount();
    for (int idx = 0; idx < batch_count; idx++)
    {
        m_tempMesh.resetMesh();
        m_circles.buildBatchMesh(idx, m_tempMesh);
        m_ringbuffer->processMesh(m_tempMesh);
    }
    m_ringbuffer->drawCurrentSegmentBuffer();
}
