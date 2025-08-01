#include "Util/ManyRectangles.h"

#include <cmath>
#include <random>


ManyRectangles::ManyRectangles()
{
    m_color_table[0] = { 255, 0, 0, 127 };
    m_color_table[1] = { 0, 255, 0, 127 };
    m_color_table[2] = { 0, 0, 255, 127 };
    m_color_table[3] = { 255, 255, 0, 127 };
    m_color_table[4] = { 255, 0, 255, 127 };
    m_color_table[5] = { 0, 255, 255, 127 };
    m_color_table[6] = { 255, 127, 0, 127 };
    m_color_table[7] = { 127, 0, 255, 127 };
    m_color_table[8] = { 0, 127, 255, 127 };
    m_color_table[9] = { 127, 127, 127, 127 };
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
            info.width = spacingx * 0.8f; // +random * spacingx * 0.2f;
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

void ManyRectangles::getBatchInfos(int batch_index, std::vector<RectInfo>& batch_infos)
{
    int start = batch_index * m_one_batch_num;
    int end = start + m_one_batch_num;
    if (end > m_infos.size())
    {
        end = int(m_infos.size());
    }
    batch_infos.assign(m_infos.begin() + start, m_infos.begin() + end);
}

void ManyRectangles::buildInfosToMesh(const std::vector<RectInfo>& batch_infos, bool fill, ColorMesh& mesh)
{
    constexpr int CrossNum = 5;
    int idx = 0;
    for (const RectInfo& info : batch_infos)
    {
        float xmin = info.center.x - info.width / 2;
        float xmax = info.center.x + info.width / 2;
        float ymin = info.center.y - info.height / 2;
        float ymax = info.center.y + info.height / 2;
        float spacingx = info.width / CrossNum;
        float spacingy = info.height / CrossNum;
        int color_index = idx++ % m_color_num;
        Color4uc color = m_color_table[color_index];

        for (int row = 0; row < CrossNum; row++)
        {
            float ycoord = ymin + (row + 0.5f) * spacingy;
            Point subcenter{ info.center.x, ycoord };
            buildOneRect(subcenter, info.width, spacingy * 0.5f, fill, color, mesh);
        }
        for (int column = 0; column < CrossNum; column++)
        {
            float xcoord = xmin + (column + 0.5f) * spacingx;
            Point subcenter{ xcoord, info.center.y };
            buildOneRect(subcenter, spacingx * 0.5f, info.height, fill, color, mesh);
        }
    }
}

void ManyRectangles::buildInfosToMesh2(const std::vector<RectInfo>& batch_infos, bool fill, unsigned int stipple, ColorStipMesh& mesh)
{
    constexpr int CrossNum = 5;
    int idx = 0;
    for (const RectInfo& info : batch_infos)
    {
        float xmin = info.center.x - info.width / 2;
        float xmax = info.center.x + info.width / 2;
        float ymin = info.center.y - info.height / 2;
        float ymax = info.center.y + info.height / 2;
        float spacingx = info.width / CrossNum;
        float spacingy = info.height / CrossNum;
        int color_index = idx++ % m_color_num;
        Color4uc color = m_color_table[color_index];

        for (int row = 0; row < CrossNum; row++)
        {
            float ycoord = ymin + (row + 0.5f) * spacingy;
            Point subcenter{ info.center.x, ycoord };
            buildOneRect2(subcenter, info.width, spacingy * 0.5f, fill, color, stipple, mesh);
        }
        for (int column = 0; column < CrossNum; column++)
        {
            float xcoord = xmin + (column + 0.5f) * spacingx;
            Point subcenter{ xcoord, info.center.y };
            buildOneRect2(subcenter, spacingx * 0.5f, info.height, fill, color, stipple, mesh);
        }
    }
}

void ManyRectangles::setGridSize(int num)
{
    m_numx = num;
    m_numy = num;
}

int ManyRectangles::gridSize() const
{
    return m_numx;
}

void ManyRectangles::buildOneRect(const Point& cent, float width, float height, bool fill, const Color4uc& color, ColorMesh& mesh)
{
    float xmin = cent.x - width / 2;
    float xmax = cent.x + width / 2;
    float ymin = cent.y - height / 2;
    float ymax = cent.y + height / 2;
    Point point0{ xmin, ymin };
    Point point1{ xmax, ymin };
    Point point2{ xmax, ymax };
    Point point3{ xmin, ymax };

    std::vector<ColorVertex>& vertices = mesh.verticesReference();
    std::vector<unsigned int>& indices = mesh.indicesReference();
    unsigned int base_idx = unsigned int(vertices.size());
    vertices.emplace_back(point0, color);
    vertices.emplace_back(point1, color);
    vertices.emplace_back(point2, color);
    vertices.emplace_back(point3, color);
    if (fill)
    {
        indices.emplace_back(base_idx + 0);
        indices.emplace_back(base_idx + 1);
        indices.emplace_back(base_idx + 2);
        indices.emplace_back(base_idx + 2);
        indices.emplace_back(base_idx + 3);
        indices.emplace_back(base_idx + 0);
    }
    else
    {
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

void ManyRectangles::buildOneRect2(const Point& cent, float width, float height, bool fill, const Color4uc& color, unsigned int stip, ColorStipMesh& mesh)
{
    float xmin = cent.x - width / 2;
    float xmax = cent.x + width / 2;
    float ymin = cent.y - height / 2;
    float ymax = cent.y + height / 2;
    Point point0{ xmin, ymin };
    Point point1{ xmax, ymin };
    Point point2{ xmax, ymax };
    Point point3{ xmin, ymax };

    std::vector<ColorStipVertex>& vertices = mesh.verticesReference();
    std::vector<unsigned int>& indices = mesh.indicesReference();
    unsigned int base_idx = unsigned int(vertices.size());
    vertices.emplace_back(point0, color, stip);
    vertices.emplace_back(point1, color, stip);
    vertices.emplace_back(point2, color, stip);
    vertices.emplace_back(point3, color, stip);
    if (fill)
    {
        indices.emplace_back(base_idx + 0);
        indices.emplace_back(base_idx + 1);
        indices.emplace_back(base_idx + 2);
        indices.emplace_back(base_idx + 2);
        indices.emplace_back(base_idx + 3);
        indices.emplace_back(base_idx + 0);
    }
    else
    {
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
