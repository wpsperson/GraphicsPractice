#pragma once

#include <string>
#include <vector>

class ModelData
{
public:
    ModelData(bool useNormal, bool useTexture);

    ~ModelData();

    void setVertexNum(std::size_t vertexNum);

    void addVertex(float vx, float vy, float vz);

    void addVertexNormal(float vx, float vy, float vz, float nx, float ny, float nz);

    void addVertexUV(float vx, float vy, float vz, float u, float v);

    void addVertexNormalUV(float vx, float vy, float vz, float nx, float ny, float nz, float u, float v);

    void addIndex(unsigned int idx);

    void setImageFile(const std::string& imageFile);
    const std::string& imageFile() const;

    std::size_t vertexCount();

    float* vertexBuffer();

    std::size_t indexCount();

    unsigned int *indexBuffer();

    bool hasNormal() const;

    bool hasTexture() const;

private:
    bool m_useNormal = false;
    bool m_useTexture = false;
    std::string m_imageFile;
    std::size_t m_vertexNum = 0;
    std::vector<float> m_vertexBuffer;
    std::vector<unsigned int> m_indexBuffer;
};