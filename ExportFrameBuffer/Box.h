#pragma once

#include <limits>

class Box
{
public:
    float minx = std::numeric_limits<float>::max();
    float miny = std::numeric_limits<float>::max();
    float minz = std::numeric_limits<float>::max();
    float maxx = -std::numeric_limits<float>::max();
    float maxy = -std::numeric_limits<float>::max();
    float maxz = -std::numeric_limits<float>::max();

public:
    void mergePt(float x, float y, float z);
};