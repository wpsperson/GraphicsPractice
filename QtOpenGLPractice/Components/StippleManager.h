#pragma once

#include <unordered_map>

enum class PolyStipple : unsigned char
{
    Solid = 0,
    PointHatch,
    Count,
};

enum class LineStipple : unsigned char
{
    Solid = 0,
    Hidden,
    Phantom,
    Dotted,
    Center,
    Dashed,
    Count,
};

struct LineStipInfo
{
    unsigned short pattern = 0xffff;
    unsigned char factor = 1;
};

class StippleManager
{
public:
    StippleManager();

    ~StippleManager();

    void initStippleManger();

    void setPolygonStipple(PolyStipple type);

    const LineStipInfo &getLineStipInfo(LineStipple type);

private:
    unsigned int m_point_hatch = 0;
    std::unordered_map<LineStipple, LineStipInfo> m_lineStipInfos;
};

