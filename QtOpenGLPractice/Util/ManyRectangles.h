#pragma once

#include <array>

#include "Const.h"
#include "Core/Mesh.h"
#include "Core/DefineEnum.h"

struct RectInfo
{
    Point center;
    float length;
    int color_index;
    LineStipple line_stip;
};

class ManyRectangles
{
public:
//    ManyRectangles();
//
//    ~ManyRectangles();
//
//    void setCircleNumInBatch(int num);
//
//    void setRange(float extent_range);
//
//    void rebuildInfos();
//
//    void rebuildRandomInfos();
//
//    int batchCount();
//
//    void buildBatchFillMesh(int batch_index, ColorMesh& mesh);
//
//    void buildBatchLineMesh(int batch_index, ColorMesh& mesh);
//
//    void setCircleNumDim(int num);
//
//private:
//    float m_Range = 2.0;
//    int NUMX = 100;
//    int NUMY = 100;
//    int NUMCOLOR = 10;
//    std::array<Color4uc, 10> TABLE_COLORS;
//
//    std::vector<RectInfo> circle_infos;
//    int index = 0;
//    int CircleNumInBatch = 10;
};