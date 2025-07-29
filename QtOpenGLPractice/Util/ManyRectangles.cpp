#include "Util/ManyRectangles.h"

#include <cmath>
#include <random>


ManyRectangles::ManyRectangles()
{
    m_color_table[0] = { 255, 0, 0, 255 };
    m_color_table[1] = { 0, 255, 0, 255 };
    m_color_table[2] = { 0, 0, 255, 255 };
    m_color_table[3] = { 255, 255, 0, 255 };
    m_color_table[4] = { 255, 0, 255, 255 };
    m_color_table[5] = { 0, 255, 255, 255 };
    m_color_table[6] = { 255, 127, 0, 255 };
    m_color_table[7] = { 127, 0, 255, 255 };
    m_color_table[8] = { 0, 127, 255, 255 };
    m_color_table[9] = { 127, 127, 127, 255 };
}

ManyRectangles::~ManyRectangles()
{
}

void ManyRectangles::setRectNumInBatch(int num)
{
    m_one_batch_num = num;
}

void ManyRectangles::setRange(float extent_range)
{
    m_distribRange = extent_range;
}

void ManyRectangles::rebuildRandomInfos()
{
    std::random_device device;
    std::mt19937 engine;
    engine.seed(device());
    std::normal_distribution<float> norm_dist(0.0, 1);

    m_infos.clear();
    float xbegin = -m_distribRange / 2;
    float ybegin = -m_distribRange / 2;
    float spacingx = m_distribRange / m_numx;
    float spacingy = m_distribRange / m_numy;
    for (int y = 0; y < m_numy; ++y)
    {
        for (int x = 0; x < m_numx; ++x)
        {
            RectInfo info;
            float ptx = xbegin + (x + 0.5f) * spacingx;
            float pty = ybegin + (y + 0.5f) * spacingy;
            float random = norm_dist(engine);
            info.center = { ptx, pty };
            info.width = spacingx * 0.4f + random * spacingx * 0.1f;
            info.height = info.width;
            info.color_index = (x + y) % 10;
            m_infos.push_back(info);
        }
    }
}

int ManyRectangles::batchCount()
{
    int unit_num = int(m_infos.size());
    if (unit_num % m_one_batch_num == 0)
    {
        return unit_num / m_one_batch_num;
    }
    else
    {
        return unit_num / m_one_batch_num + 1;
    }
}

void ManyRectangles::buildBatchFillMesh(int batch_index, ColorMesh& mesh)
{
    int start = batch_index * m_one_batch_num;
    int end = start + m_one_batch_num;
    if (end > m_infos.size())
    {
        end = int(m_infos.size());
    }
    std::vector<ColorVertex>& vertices = mesh.verticesReference();
    std::vector<unsigned int>& indices = mesh.indicesReference();
    unsigned int base_idx = unsigned int(vertices.size());
    for (int idx = start; idx < end; idx++)
    {

    }
}

void ManyRectangles::buildBatchLineMesh(int batch_index, ColorMesh& mesh)
{
    int start = batch_index * m_one_batch_num;
    int end = start + m_one_batch_num;
    if (end > m_infos.size())
    {
        end = int(m_infos.size());
    }

    std::vector<ColorVertex>& vertices = mesh.verticesReference();
    std::vector<unsigned int>& indices = mesh.indicesReference();
    unsigned int base_idx = unsigned int(vertices.size());
    for (int idx = start; idx < end; idx++)
    {
        const RectInfo& info = m_infos[idx];
        float xmin = info.center.x - info.width / 2;
        float xmax = info.center.x + info.width / 2;
        float ymin = info.center.y - info.height / 2;
        float ymax = info.center.y + info.height / 2;
        Point point0{ xmin, ymin };
        Point point1{ xmax, ymin };
        Point point2{ xmax, ymax };
        Point point3{ xmin, ymax };
        int color_index = idx % m_color_num;
        Color4uc color = m_color_table[color_index];
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
        base_idx += 4;
    }
}

void ManyRectangles::setGridSize(int num)
{
    m_numx = num;
    m_numy = num;
}
