#pragma once

enum class PolyStipple : unsigned char
{
    Solid = 0,
    PointHatch,
    Count,
};


class StippleManager
{
public:
    StippleManager();

    ~StippleManager();

    void initStippleManger();

    void setPolygonStipple(PolyStipple type);

    void closePolygonStipple();

private:
    unsigned int m_point_hatch = 0;
};

