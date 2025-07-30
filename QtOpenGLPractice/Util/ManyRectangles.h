#pragma once

#include <array>

#include "Core/DefineStruct.h"
#include "Core/Mesh.h"
#include "Core/DefineEnum.h"

class ManyRectangles
{
public:
    ManyRectangles();

    ~ManyRectangles();

    void setRectNumInBatch(int num);

    void setRange(float extent_range);

    void rebuildRandomInfos();

    int batchCount();

    void buildBatchFillMesh(int batch_index, ColorMesh& mesh);

    void buildBatchLineMesh(int batch_index, ColorMesh& mesh);

    void setGridSize(int num);

    static void buildOneRect(const Point& cent, float width, float height, bool fill, const Color4uc& color, ColorMesh& mesh);

private:
    float m_distribRange = 2.0;
    int m_numx = 100;
    int m_numy = 100;
    const int m_color_num = 10;
    std::array<Color4uc, 10> m_color_table;
    std::vector<RectInfo> m_infos;
    int m_one_batch_num = 10; // how may rectangles in one build batch.
};