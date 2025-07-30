#include "Performance/StippleOperation.h"


#include "Core/OpenGLHeader.h"
#include "Components/Renderer.h"
#include "Components/ViewPort.h"
#include "Components/ProgramManager.h"
#include "Util/Utils.h"
#include "Util/ArgumentUtil.h"

StippleOperation::StippleOperation()
{
    // arguments:  StippleOperation  GridSize=100
    int value = 0;
    if (ArgumentUtil::getValueByKey("GridSize", value))
    {
        m_circles.setGridSize(value);
        m_rects.setGridSize(value);
    }
}

StippleOperation::~StippleOperation()
{
    delete m_ringbuffer;
}

void StippleOperation::initialize(Renderer* renderer) noexcept
{
    renderer->viewPort()->setDesign(-hrange, -hrange, 2 * hrange, 2 * hrange);

    m_ringbuffer = new PersistMapRingBuffer;
    m_ringbuffer->initBuffer();
    m_stip_mgr = new StippleManager;
    m_stip_mgr->initStippleManger();

    m_circles.setRange(float(hrange));
    m_circles.setCircleNumInBatch(10);
    m_rects.setRange(float(hrange));
    m_rects.setRectNumInBatch(100);
}

void StippleOperation::paint(Renderer* renderer) noexcept
{
    /*
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
        m_circles.buildBatchFillMesh(idx, m_tempMesh);
        m_ringbuffer->processMesh(m_tempMesh);
    }
    m_ringbuffer->drawCurrentSegmentBuffer();
    */

    m_rects.rebuildRandomInfos();
    ProgramManager* progMgr = renderer->programMgr();
    progMgr->applyProgram(ProgramType::LineStipple);
    ViewBox viewbox = Utils::toViewBox(renderer->viewPort()->getView());
    llPoint screen = renderer->viewPort()->getViewPort();
    const LineStipInfo& info = m_stip_mgr->getLineStipInfo(LineStipple::Phantom);
    progMgr->uniformViewBox(viewbox);
    progMgr->uniform2f("u_resolution", float(screen.x), float(screen.y));
    progMgr->uniform1f("u_factor", float(info.factor));
    progMgr->uniform1ui("u_pattern", unsigned int(info.pattern));

    m_ringbuffer->bindVAO();
    m_ringbuffer->setCurrentDrawMode(DrawMode::Lines);
    int batch_count = m_rects.batchCount();
    for (int idx = 0; idx < batch_count; idx++)
    {
        m_tempMesh.resetMesh();
        m_rects.buildBatchLineMesh(idx, m_tempMesh);
        m_ringbuffer->processMesh(m_tempMesh);
    }
    m_ringbuffer->drawCurrentSegmentBuffer();

}

