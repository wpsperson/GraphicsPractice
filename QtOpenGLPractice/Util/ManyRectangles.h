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

    void getBatchInfos(int batch_index, std::vector<RectInfo>& batch_infos);

    void buildInfosToMesh(const std::vector<RectInfo> &batch_infos, bool fill, ColorMesh& mesh);

    void buildInfosToMesh2(const std::vector<RectInfo>& batch_infos, bool fill, unsigned int stipple, ColorStipMesh& mesh);

    void setGridSize(int num);

    int gridSize() const;

    static void buildOneRect(const Point& cent, float width, float height, bool fill, const Color4uc& color, ColorMesh& mesh);

    static void buildOneRect2(const Point& cent, float width, float height, bool fill, const Color4uc& color, unsigned int stip, ColorStipMesh& mesh);

private:
    float m_distribRange = 2.0;
    int m_numx = 100;
    int m_numy = 100;
    const int m_color_num = 10;
    std::array<Color4uc, 10> m_color_table;
    std::vector<RectInfo> m_infos;
    int m_one_batch_num = 10; // how may rectangles in one build batch.
};