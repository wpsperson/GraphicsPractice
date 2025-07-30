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
    m_one_batch_num = num;
}

void ManyCircles::setRange(float extent_range)
{
    m_range = extent_range;
}

void ManyCircles::rebuildInfos()
{
    m_circle_infos.clear();
    float xbegin = -m_range / 2;
    float ybegin = -m_range / 2;
    float spacingx = m_range / m_numx;
    float spacingy = m_range / m_numx;
    for (int y = 0; y < m_numy; ++y)
    {
        for (int x = 0; x < m_numx; ++x)
        {
            CircleInfo info;
            float ptx = xbegin + (x + 0.5f) * spacingx;
            float pty = ybegin + (y + 0.5f) * spacingy;
            info.center = {ptx, pty};
            info.radius = spacingx * 0.4f;
            info.color_index = (x + y) % 10;
            m_circle_infos.push_back(info);
        }
    }
}

void ManyCircles::rebuildRandomInfos()
{
    std::random_device device;
    std::mt19937 engine;
    engine.seed(device());
    std::normal_distribution<float> norm_dist(0.0, 1);

    m_circle_infos.clear();
    float xbegin = -m_range / 2;
    float ybegin = -m_range / 2;
    float spacingx = m_range / m_numx;
    float spacingy = m_range / m_numx;
    for (int y = 0; y < m_numy; ++y)
    {
        for (int x = 0; x < m_numx; ++x)
        {
            CircleInfo info;
            float ptx = xbegin + (x + 0.5f) * spacingx;
            float pty = ybegin + (y + 0.5f) * spacingy;
            float random = norm_dist(engine);
            info.center = { ptx, pty };
            info.radius = spacingx * 0.4f + random * spacingx * 0.1f;
            info.color_index = (x + y) % 10;
            m_circle_infos.push_back(info);
        }
    }
}

int ManyCircles::batchCount()
{
    int circle_num = int(m_circle_infos.size());
    if (circle_num % m_one_batch_num == 0)
    {
        return circle_num / m_one_batch_num;
    }
    else
    {
        return circle_num / m_one_batch_num + 1;
    }
}

void ManyCircles::buildBatchFillMesh(int batch_index, ColorMesh& mesh)
{
    int start = batch_index * m_one_batch_num;
    int end = start + m_one_batch_num;
    if (end > m_circle_infos.size())
    {
        end = int(m_circle_infos.size());
    }

    std::vector<ColorVertex>& vertices = mesh.verticesReference();
    std::vector<unsigned int>& indices = mesh.indicesReference();
    unsigned int base_idx = unsigned int(vertices.size());
    for (int idx = start; idx < end; idx++)
    {
        const CircleInfo& info = m_circle_infos[idx];
        ColorVertex vert0;
        vert0.point = info.center;
        vert0.color = TABLE_COLORS[info.color_index];
        vertices.emplace_back(vert0);

        for (int i = 0; i < m_resolution; ++i)
        {
            float angle = float(2.0f * kMathPId * i) / m_resolution;
            float px = info.center.x + std::cos(angle) * info.radius;
            float py = info.center.y + std::sin(angle) * info.radius;

            ColorVertex vert;
            vert.point = Point(px, py);
            vert.color = TABLE_COLORS[info.color_index];
            vertices.emplace_back(vert);
        }

        for (int i = 0; i < m_resolution; ++i)
        {
            indices.push_back(base_idx);
            indices.push_back(base_idx + i + 1);
            indices.push_back(base_idx + ((i + 1) % m_resolution) + 1);
        }
        base_idx += m_resolution + 1;
    }
}

void ManyCircles::buildBatchLineMesh(int batch_index, ColorMesh& mesh)
{
    int start = batch_index * m_one_batch_num;
    int end = start + m_one_batch_num;
    if (end > m_circle_infos.size())
    {
        end = int(m_circle_infos.size());
    }

    std::vector<ColorVertex>& vertices = mesh.verticesReference();
    std::vector<unsigned int>& indices = mesh.indicesReference();
    unsigned int base_idx = unsigned int(vertices.size());
    for (int idx = start; idx < end; idx++)
    {
        const CircleInfo& info = m_circle_infos[idx];
        for (int i = 0; i < m_resolution; ++i)
        {
            float angle = float(2.0f * kMathPId * i) / m_resolution;
            float px = info.center.x + std::cos(angle) * info.radius;
            float py = info.center.y + std::sin(angle) * info.radius;
            ColorVertex vert;
            vert.point = Point(px, py);
            vert.color = TABLE_COLORS[info.color_index];
            vertices.emplace_back(vert);
        }
        for (int i = 0; i < m_resolution; ++i)
        {
            indices.push_back(base_idx + i);
            indices.push_back(base_idx + ((i + 1) % m_resolution));
        }
        base_idx += m_resolution;
    }
}

void ManyCircles::setGridSize(int num)
{
    m_numx = num;
    m_numy = num;
}
