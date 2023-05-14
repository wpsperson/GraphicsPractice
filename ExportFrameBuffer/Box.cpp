#include "Box.h"

void Box::mergePt(float x, float y, float z)
{
    if (x < minx)
    {
        minx = x;
    }
    if (x > maxx)
    {
        maxx = x;
    }

    if (y < miny)
    {
        miny = y;
    }
    if (y > maxy)
    {
        maxy = y;
    }

    if (z < minz)
    {
        minz = z;
    }
    if (z > maxz)
    {
        maxz = z;
    }
}
