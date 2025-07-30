#pragma once

#include <array>

#include "Core/DefineStruct.h"
#include "Core/Mesh.h"

struct CircleInfo {
    Point center;
    float radius;
    int color_index;
};

class ManyCircles
{
public:
    ManyCircles();

    ~ManyCircles();

    void setCircleNumInBatch(int num);

    void setRange(float extent_range);

    void rebuildInfos();

    void rebuildRandomInfos();

    int batchCount();

    void buildBatchFillMesh(int batch_index, ColorMesh& mesh);

    void buildBatchLineMesh(int batch_index, ColorMesh& mesh);

    void setGridSize(int num);

private:
    float m_range = 2.0;
    int m_numx = 100;
    int m_numy = 100;
    int m_resolution = 100;
    const int m_color_num = 10;
    std::array<Color4uc, 10> TABLE_COLORS;

    std::vector<CircleInfo> m_circle_infos;
    int m_one_batch_num = 10;
};

