#pragma once

#include <vector>
#include "Core/DefineStruct.h"

class MeshBuilder
{
public:
    static std::vector<Point> generateCircle(float centx, float centy, float radius, int num);
};


