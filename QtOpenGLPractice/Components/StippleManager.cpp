#include "Components/StippleManager.h"

#include <array>
#include "Core/OpenGLHeader.h"
#include "Data/StippleConst.h"
#include "Util/Utils.h"


StippleManager::StippleManager()
{

}

StippleManager::~StippleManager()
{
}

void StippleManager::initStippleManger()
{
    // line stipple
    m_lineStipInfos[LineStipple::Solid] = LineStipInfo{ kLineSolid, kFactorSolid };
    m_lineStipInfos[LineStipple::Hidden] = LineStipInfo{ kLineHidden, kFactorHidden };
    m_lineStipInfos[LineStipple::Phantom] = LineStipInfo{ kLinePhantom, kFactorPhantom };
    m_lineStipInfos[LineStipple::Dotted] = LineStipInfo{ kLineDotted, kFactorDotted };
    m_lineStipInfos[LineStipple::Center] = LineStipInfo{ kLineCenter, kFactorCenter };
    m_lineStipInfos[LineStipple::Dashed] = LineStipInfo{ kLineDashed, kFactorDashed };
    // polygon stipple
    m_polyStipInfos[PolyStipple::Solid] = PolyStipInfo{ kSolidPattern, 0 };
    m_polyStipInfos[PolyStipple::PointHatch] = PolyStipInfo{ kPointHatch, 0 };
    m_polyStipInfos[PolyStipple::LeftSlash] = PolyStipInfo{ kLeftSlash, 0 };
    m_polyStipInfos[PolyStipple::RightSlash] = PolyStipInfo{ kRightSlash, 0 };
    m_polyStipInfos[PolyStipple::HorizonLine] = PolyStipInfo{ kHorizonLine, 0 };
    m_polyStipInfos[PolyStipple::VerticalLine] = PolyStipInfo{ kVerticalLine, 0 };
    m_polyStipInfos[PolyStipple::Cross] = PolyStipInfo{ kCross, 0 };
    m_polyStipInfos[PolyStipple::CrossHatch] = PolyStipInfo{ kCrossHatch, 0 };
    m_polyStipInfos[PolyStipple::HorizonStrip] = PolyStipInfo{ KHorizonStrip8, 0 };
    m_polyStipInfos[PolyStipple::VerticalStrip] = PolyStipInfo{ KVerticalStrip8, 0 };
    m_polyStipInfos[PolyStipple::InterleaveRect] = PolyStipInfo{ kInterleaveRect42, 0 };

    buildPolygonTexture();
}

void StippleManager::setPolygonStipple(PolyStipple type)
{
    const PolyStipInfo& info = m_polyStipInfos.at(type);
    glBindTexture(GL_TEXTURE_2D, info.texture);
}

const LineStipInfo &StippleManager::getLineStipInfo(LineStipple type)
{
    return m_lineStipInfos.at(type);
}

unsigned int StippleManager::mergeFactorPattern(const LineStipInfo& stipple_info)
{
    unsigned int result = (stipple_info.factor << 16) | stipple_info.pattern;
    return result;
}

void StippleManager::buildPolygonTexture()
{
    constexpr int STIP_WIDTH = 32;
    constexpr int STIP_HEIGHT = 32;
    std::array<unsigned char, 1024> buffer;

    for (auto& [type, info] : m_polyStipInfos)
    {
        glGenTextures(1, &info.texture);
        glBindTexture(GL_TEXTURE_2D, info.texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        Utils::bitToUchars(info.data, buffer);
        const unsigned char* data = buffer.data();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, STIP_WIDTH, STIP_HEIGHT, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
    }
    glActiveTexture(GL_TEXTURE0);
}
