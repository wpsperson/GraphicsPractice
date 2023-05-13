#include "ModelData.h"
#include "GLObject.h"

ModelData::ModelData(bool useNormal, bool useTexture) : m_useNormal(useNormal), m_useTexture(useTexture)
{
}

ModelData::~ModelData()
{
}

void ModelData::setVertexNum(std::size_t vertexNum)
{
    m_vertexNum = vertexNum;
    std::size_t vectorSize = m_vertexNum * GLObject::oneVertexFloatCount(m_useNormal, m_useTexture);
    m_vertexBuffer.reserve(vectorSize);
}

void ModelData::addVertex(float vx, float vy, float vz)
{
    m_vertexBuffer.emplace_back(vx);
    m_vertexBuffer.emplace_back(vy);
    m_vertexBuffer.emplace_back(vz);
}

void ModelData::addVertexNormal(float vx, float vy, float vz, float nx, float ny, float nz)
{
    m_vertexBuffer.emplace_back(vx);
    m_vertexBuffer.emplace_back(vy);
    m_vertexBuffer.emplace_back(vz);
    m_vertexBuffer.emplace_back(nx);
    m_vertexBuffer.emplace_back(ny);
    m_vertexBuffer.emplace_back(nz);
}

void ModelData::addVertexUV(float vx, float vy, float vz, float u, float v)
{
    m_vertexBuffer.emplace_back(vx);
    m_vertexBuffer.emplace_back(vy);
    m_vertexBuffer.emplace_back(vz);
    m_vertexBuffer.emplace_back(u);
    m_vertexBuffer.emplace_back(v);
}

void ModelData::addVertexNormalUV(float vx, float vy, float vz, float nx, float ny, float nz, float u, float v)
{
    m_vertexBuffer.emplace_back(vx);
    m_vertexBuffer.emplace_back(vy);
    m_vertexBuffer.emplace_back(vz);
    m_vertexBuffer.emplace_back(nx);
    m_vertexBuffer.emplace_back(ny);
    m_vertexBuffer.emplace_back(nz);
    m_vertexBuffer.emplace_back(u);
    m_vertexBuffer.emplace_back(v);
}

void ModelData::addIndex(unsigned int idx)
{
    m_indexBuffer.emplace_back(idx);
}

void ModelData::setImageFile(const std::string& imageFile)
{
    m_imageFile = imageFile;
}

const std::string& ModelData::imageFile() const
{
    return m_imageFile;
}

std::size_t ModelData::vertexArrayBufferSize()
{
    return m_vertexBuffer.size() * sizeof(float);
}

float* ModelData::vertexBuffer()
{
    return m_vertexBuffer.data();
}

std::size_t ModelData::indexArrayBufferSize()
{
    return m_indexBuffer.size() * sizeof(unsigned int);
}

unsigned int *ModelData::indexBuffer()
{
    return m_indexBuffer.data();
}
