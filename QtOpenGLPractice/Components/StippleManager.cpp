#include "Components/StippleManager.h"

#include <array>
#include "OpenGLHeader.h"
#include "Components/StippleConst.h"
#include "Util/Utils.h"


StippleManager::StippleManager()
{

}

StippleManager::~StippleManager()
{
}

void StippleManager::initStippleManger()
{
    constexpr int STIP_WIDTH = 32;
    constexpr int STIP_HEIGHT = 32;
    glGenTextures(1, &m_point_hatch);
    glBindTexture(GL_TEXTURE_2D, m_point_hatch);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::array<unsigned char, 1024> buffer;
    Utils::bitToUchars(kPointHatch, buffer);
    const unsigned char* data = buffer.data();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, STIP_WIDTH, STIP_HEIGHT, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);

    glActiveTexture(GL_TEXTURE0);
}

void StippleManager::setPolygonStipple(PolyStipple type)
{
    glBindTexture(GL_TEXTURE_2D, m_point_hatch);
}

void StippleManager::closePolygonStipple()
{
}
