#pragma once

#include <map>
#include "Core/DefineEnum.h"

struct LineStipInfo
{
    unsigned short pattern = 0xffff;
    unsigned char factor = 1;
};

struct PolyStipInfo
{
    const unsigned char* data = nullptr;
    unsigned int texture = 0;
};

class StippleManager
{
public:
    StippleManager();

    ~StippleManager();

    void initStippleManger();

    void setPolygonStipple(PolyStipple type);

    const LineStipInfo &getLineStipInfo(LineStipple type);

    static unsigned int mergeFactorPattern(const LineStipInfo& stipple_info);

private:
    void buildPolygonTexture();

private:
    std::map<PolyStipple, PolyStipInfo> m_polyStipInfos;
    std::map<LineStipple, LineStipInfo> m_lineStipInfos;
};

