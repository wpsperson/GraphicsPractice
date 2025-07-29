#include "Util/ManyCircles.h"

#include <cmath>
#include <random>

#include "Core/DefineConst.h"

ManyCircles::ManyCircles()
{
    TABLE_COLORS[0] = {255, 0, 0, 255};
    TABLE_COLORS[1] = {0, 255, 0, 255};
    TABLE_COLORS[2] = {0, 0, 255, 255};
    TABLE_COLORS[3] = {255, 255, 0, 255};
    TABLE_COLORS[4] = {255, 0, 255, 255};
    TABLE_COLORS[5] = {0, 255, 255, 255};
    TABLE_COLORS[6] = {255, 127, 0, 255};
    TABLE_COLORS[7] = {127, 0, 255, 255};
    TABLE_COLORS[8] = {0, 127, 255, 255};
    TABLE_COLORS[9] = {127, 127, 127, 255};
}

ManyCircles::~ManyCircles()
{
}

void ManyCircles::setCircleNumInBatch(int num)
{
    CircleNumInBatch = num;
}

void ManyCircles::setRange(float extent_range)
{
    m_Range = extent_range;
}

void ManyCircles::rebuildInfos()
{
    circle_infos.clear();
    float xbegin = -m_Range / 2;
    float ybegin = -m_Range / 2;
    float spacingx = m_Range / NUMX;
    float spacingy = m_Range / NUMX;
    for (int y = 0; y < NUMY; ++y)
    {
        for (int x = 0; x < NUMX; ++x)
        {
            CircleInfo info;
            float ptx = xbegin + (x + 0.5f) * spacingx;
            float pty = ybegin + (y + 0.5f) * spacingy;
            info.center = {ptx, pty};
            info.radius = spacingx * 0.4f;
            info.color_index = (x + y) % 10;
            circle_infos.push_back(info);
        }
    }
}

void ManyCircles::rebuildRandomInfos()
{
    std::random_device device;
    std::mt19937 engine;
    engine.seed(device());
    std::normal_distribution<float> norm_dist(0.0, 1);

    circle_infos.clear();
    float xbegin = -m_Range / 2;
    float ybegin = -m_Range / 2;
    float spacingx = m_Range / NUMX;
    float spacingy = m_Range / NUMX;
    for (int y = 0; y < NUMY; ++y)
    {
        for (int x = 0; x < NUMX; ++x)
        {
            CircleInfo info;
            float ptx = xbegin + (x + 0.5f) * spacingx;
            float pty = ybegin + (y + 0.5f) * spacingy;
            float random = norm_dist(engine);
            info.center = { ptx, pty };
            info.radius = spacingx * 0.4f + random * spacingx * 0.1f;
            info.color_index = (x + y) % 10;
            circle_infos.push_back(info);
        }
    }
}

int ManyCircles::batchCount()
{
    int circle_num = int(circle_infos.size());
    if (circle_num % CircleNumInBatch == 0)
    {
        return circle_num / CircleNumInBatch;
    }
    else
    {
        return circle_num / CircleNumInBatch + 1;
    }
}

void ManyCircles::buildBatchFillMesh(int batch_index, ColorMesh& mesh)
{
    int start = batch_index * CircleNumInBatch;
    int end = start + CircleNumInBatch;
    if (end > circle_infos.size())
    {
        end = int(circle_infos.size());
    }

    std::vector<ColorVertex>& vertices = mesh.verticesReference();
    std::vector<unsigned int>& indices = mesh.indicesReference();
    unsigned int base_idx = unsigned int(vertices.size());
    for (int idx = start; idx < end; idx++)
    {
        const CircleInfo& info = circle_infos[idx];
        ColorVertex vert0;
        vert0.point = info.center;
        vert0.color = TABLE_COLORS[info.color_index];
        vertices.emplace_back(vert0);

        for (int i = 0; i < CIRCLE_RESO; ++i)
        {
            float angle = float(2.0f * kMathPId * i) / CIRCLE_RESO;
            float px = info.center.x + std::cos(angle) * info.radius;
            float py = info.center.y + std::sin(angle) * info.radius;

            ColorVertex vert;
            vert.point = Point(px, py);
            vert.color = TABLE_COLORS[info.color_index];
            vertices.emplace_back(vert);
        }

        for (int i = 0; i < CIRCLE_RESO; ++i)
        {
            indices.push_back(base_idx);
            indices.push_back(base_idx + i + 1);
            indices.push_back(base_idx + ((i + 1) % CIRCLE_RESO) + 1);
        }
        base_idx += CIRCLE_RESO + 1;
    }
}

void ManyCircles::buildBatchLineMesh(int batch_index, ColorMesh& mesh)
{
    int start = batch_index * CircleNumInBatch;
    int end = start + CircleNumInBatch;
    if (end > circle_infos.size())
    {
        end = int(circle_infos.size());
    }

    std::vector<ColorVertex>& vertices = mesh.verticesReference();
    std::vector<unsigned int>& indices = mesh.indicesReference();
    unsigned int base_idx = unsigned int(vertices.size());
    for (int idx = start; idx < end; idx++)
    {
        const CircleInfo& info = circle_infos[idx];
        for (int i = 0; i < CIRCLE_RESO; ++i)
        {
            float angle = float(2.0f * kMathPId * i) / CIRCLE_RESO;
            float px = info.center.x + std::cos(angle) * info.radius;
            float py = info.center.y + std::sin(angle) * info.radius;
            ColorVertex vert;
            vert.point = Point(px, py);
            vert.color = TABLE_COLORS[info.color_index];
            vertices.emplace_back(vert);
        }
        for (int i = 0; i < CIRCLE_RESO; ++i)
        {
            indices.push_back(base_idx + i);
            indices.push_back(base_idx + ((i + 1) % CIRCLE_RESO));
        }
        base_idx += CIRCLE_RESO;
    }
}

void ManyCircles::setCircleNumDim(int num)
{
    NUMX = num;
    NUMY = num;
}
