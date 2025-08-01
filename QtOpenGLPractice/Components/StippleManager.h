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

    void usePolygonStippleTextureArray();

    const LineStipInfo &getLineStipInfo(LineStipple type);

    static unsigned int mergeFactorPattern(const LineStipInfo& stipple_info);

private:
    void buildPolygonTexture();

    void buildTextureArray();

private:
    unsigned int m_texture_array_id = 0;
    std::map<PolyStipple, PolyStipInfo> m_polyStipInfos;
    std::map<LineStipple, LineStipInfo> m_lineStipInfos;
};

