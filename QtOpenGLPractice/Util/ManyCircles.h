#pragma once

#include <array>

#include "Const.h"
#include "Model/Mesh.h"

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

    void setCircleNumDim(int num);

private:
    float m_Range = 2.0;
    int NUMX = 100;
    int NUMY = 100;
    int CIRCLE_RESO = 100;
    int NUMCOLOR = 10;
    std::array<Color4uc, 10> TABLE_COLORS;

    std::vector<CircleInfo> circle_infos;
    int index = 0;
    int CircleNumInBatch = 10;
};

