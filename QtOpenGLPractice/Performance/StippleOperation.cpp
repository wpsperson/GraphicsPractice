#include "Performance/StippleOperation.h"


#include "Core/OpenGLHeader.h"
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

    ProgramManager* progMgr = renderer->programMgr();
    progMgr->applyProgram(ProgramType::LineStipple);
    ViewBox viewbox = Utils::toViewBox(renderer->viewPort()->getView());
    llPoint screen = renderer->viewPort()->getViewPort();
    const LineStipInfo& info = m_stip_mgr->getLineStipInfo(LineStipple::Solid);
    progMgr->uniformViewBox(viewbox);
    progMgr->uniform2f("u_resolution", float(screen.x), float(screen.y));
    progMgr->uniform1f("u_factor", float(info.factor));
    progMgr->uniform1ui("u_pattern", unsigned int(info.pattern));

    m_ringbuffer->bindVAO();

    m_tempMesh.resetMesh();
    buildRectangles(hrange, m_tempMesh);
    m_ringbuffer->processMesh(m_tempMesh);
    m_ringbuffer->drawCurrentSegmentLines();

}

void StippleOperation::buildRectangles(glint64 half_range, ColorMesh& mesh)
{
    constexpr int COLORNUM = 10;
    std::array<Color4uc, COLORNUM> color_table;
    color_table[0] = { 255, 0, 0, 255 };
    color_table[1] = { 0, 255, 0, 255 };
    color_table[2] = { 0, 0, 255, 255 };
    color_table[3] = { 255, 255, 0, 255 };
    color_table[4] = { 255, 0, 255, 255 };
    color_table[5] = { 0, 255, 255, 255 };
    color_table[6] = { 255, 127, 0, 255 };
    color_table[7] = { 127, 0, 255, 255 };
    color_table[8] = { 0, 127, 255, 255 };
    color_table[9] = { 127, 127, 127, 255 };

    const int rect_num = 4;
    float range = float(half_range * 2);
    float spacingx = range / rect_num;
    float spacingy = range / rect_num;
    float xbegin = -range / 2;
    float ybegin = -range / 2;
    float hw = spacingx * 0.25f; // half width
    float hh = spacingy * 0.25f; // half height
    std::vector<ColorVertex>& vertices = mesh.verticesReference();
    std::vector<unsigned int>& indices = mesh.indicesReference();
    for (int row = 0; row < rect_num; row++)
    {
        for (int col = 0; col < rect_num; col++)
        {
            float xx = xbegin + (col + 0.5f) * spacingx;
            float yy = ybegin + (row + 0.5f) * spacingy;
            unsigned int base_idx = unsigned int(vertices.size());
            float xmin = xx - hw;
            float xmax = xx + hw;
            float ymin = yy - hh;
            float ymax = yy + hh;
            Point point0{ xmin, ymin };
            Point point1{ xmax, ymin };
            Point point2{ xmax, ymax };
            Point point3{ xmin, ymax };
            int color_index = (row * rect_num + col) % COLORNUM;
            Color4uc color = color_table[color_index];
            vertices.emplace_back(point0, color);
            vertices.emplace_back(point1, color);
            vertices.emplace_back(point2, color);
            vertices.emplace_back(point3, color);
            indices.emplace_back(base_idx + 0);
            indices.emplace_back(base_idx + 1);
            indices.emplace_back(base_idx + 1);
            indices.emplace_back(base_idx + 2);
            indices.emplace_back(base_idx + 2);
            indices.emplace_back(base_idx + 3);
            indices.emplace_back(base_idx + 3);
            indices.emplace_back(base_idx + 0);
        }
    }
}
