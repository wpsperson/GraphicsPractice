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

    void rebuildInfos();

    void rebuildRandomInfos();

    int batchCount();

    void buildBatchMesh(int batch_index, ColorMesh& mesh);

private:
    const int NUMX = 100;
    const int NUMY = 100;
    const int NUMSEGMENT = 16;
    const int NUMCOLOR = 10;
    std::array<Color4uc, 10> TABLE_COLORS;

    std::vector<CircleInfo> circle_infos;
    int index = 0;
    int CircleNumInBatch = 100;
};

