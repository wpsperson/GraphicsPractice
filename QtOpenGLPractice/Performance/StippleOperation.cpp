#include "Performance/StippleOperation.h"

#include <iostream>

#include "Core/OpenGLHeader.h"
#include "Components/Renderer.h"
#include "Components/ViewPort.h"
#include "Components/ProgramManager.h"
#include "Util/Utils.h"
#include "Util/ArgumentUtil.h"
#include "OpenGLWidget.h"

extern double g_cofficient;

StippleOperation::StippleOperation()
{
    // arguments:  StippleOperation  GridSize=100 Coff=10
    int value = 0;
    if (ArgumentUtil::getValueByKey("GridSize", value))
    {
        m_circles.setGridSize(value);
        m_rects.setGridSize(value);
    }
    if (ArgumentUtil::getValueByKey("Coff", value))
    {
        g_cofficient = 1.0 / value;
        std::cout << "cofficent is " << g_cofficient << std::endl;
    }
}

StippleOperation::~StippleOperation()
{
    delete m_ringbuffer;
    delete m_ringbuffer2;
}

void StippleOperation::initialize(Renderer* renderer) noexcept
{
    m_renderer = renderer;
    renderer->viewPort()->setDesign(-hrange, -hrange, 2 * hrange, 2 * hrange);

    m_ringbuffer = new PersistMapRingBuffer;
    m_ringbuffer->initBuffer();
    m_ringbuffer2 = new PersistMapStipple;
    m_ringbuffer2->initBuffer();

    m_stip_mgr = new StippleManager;
    m_stip_mgr->initStippleManger();

    m_circles.setRange(float(hrange));
    m_circles.setCircleNumInBatch(10);
    m_rects.setRange(float(hrange));
    m_rects.setRectNumInBatch(10);
}

void StippleOperation::resizeOperation(int width, int height) noexcept
{
    m_renderer->viewPort()->setView(-hrange/2, -hrange/2, hrange, hrange);
}

void StippleOperation::paint(Renderer* renderer) noexcept
{
    // method 1 line stipple using uniform.
    /*
    */
    m_rects.rebuildRandomInfos();
    ProgramManager* progMgr = renderer->programMgr();
    progMgr->applyProgram(ProgramType::LineStipple);
    ViewBox viewbox = Utils::toViewBox(renderer->viewPort()->getView());
    llPoint screen = renderer->viewPort()->getViewPort();
    double ratio = m_widget->devicePixelRatioF();
    progMgr->uniformViewBox(viewbox);
    progMgr->uniform2f("u_resolution", float(screen.x * ratio), float(screen.y * ratio));

    m_ringbuffer->bindVAO();
    m_ringbuffer->setCurrentDrawMode(DrawMode::Lines);
    std::vector<RectInfo> batch_infos;
    int batch_count = m_rects.batchCount();
    for (int idx = 0; idx < batch_count; idx++)
    {
        m_tempMesh.resetMesh();
        m_rects.getBatchInfos(idx, batch_infos);
        int count = static_cast<int>(LineStipple::Count);
        LineStipple stipple = static_cast<LineStipple>(idx % count);
        const LineStipInfo& info = m_stip_mgr->getLineStipInfo(stipple);
        progMgr->uniform1f("u_factor", float(info.factor));
        progMgr->uniform1ui("u_pattern", unsigned int(info.pattern));
        m_rects.buildInfosToMesh(batch_infos, false, m_tempMesh);
        m_ringbuffer->processMesh(m_tempMesh);
        m_ringbuffer->drawCurrentSegmentBuffer();
    }
    

    // method 2. line stipple using vertex attribute.
    /*
    m_rects.rebuildRandomInfos();
    ProgramManager* progMgr = renderer->programMgr();
    progMgr->applyProgram(ProgramType::LineStippleAttribute);
    ViewBox viewbox = Utils::toViewBox(renderer->viewPort()->getView());
    llPoint screen = renderer->viewPort()->getViewPort();
    double ratio = m_widget->devicePixelRatioF();
    progMgr->uniformViewBox(viewbox);
    progMgr->uniform2f("u_resolution", float(screen.x * ratio), float(screen.y * ratio));

    m_ringbuffer2->bindVAO();
    m_ringbuffer2->setCurrentDrawMode(DrawMode::Lines);
    int batch_count = m_rects.batchCount();
    std::vector<RectInfo> batch_infos;
    for (int idx = 0; idx < batch_count; idx++)
    {
        m_tempMesh2.resetMesh();
        m_rects.getBatchInfos(idx, batch_infos);
        int count = static_cast<int>(LineStipple::Count);
        LineStipple stipple = static_cast<LineStipple>(idx % count);
        const LineStipInfo& info = m_stip_mgr->getLineStipInfo(stipple);
        unsigned int nstipple = StippleManager::mergeFactorPattern(info);
        m_rects.buildInfosToMesh2(batch_infos, false, nstipple, m_tempMesh2);
        m_ringbuffer2->processMesh(m_tempMesh2);
    }
    m_ringbuffer2->drawCurrentSegmentBuffer();
    */

    // method 3 
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


}

