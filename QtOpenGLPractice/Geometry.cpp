#include "Geometry.h"

#include <cmath>


std::vector<Point> generateCircle(float centx, float centy, float radius, int num)
{
    std::vector<Point> circlePoints;
    circlePoints.reserve(num);
    float step = float(M_PI * 2) / num;
    for (int i = 0; i < num; i++)
    {
        float angle = i * step;
        Point pt;
        pt.x = centx + radius * std::cos(angle);
        pt.y = centy + radius * std::sin(angle);
        circlePoints.emplace_back(pt);
    }

    std::vector<Point> triangles;
    triangles.reserve(num * 3);
    for (int i = 0; i < num; i++)
    {
        triangles.emplace_back(Point{ centx, centy });
        triangles.emplace_back(circlePoints[i]);
        if (i == num - 1)
        {
            triangles.emplace_back(circlePoints[0]);
        }
        else
        {
            triangles.emplace_back(circlePoints[i + 1]);
        }
    }
    return triangles;
}
