#pragma once

#include <cstddef>

class GLObject
{
public:
    GLObject();
    ~GLObject();

    void createVertexArrayObject();

    void createVertexBufferObject(float* vertexArray, std::size_t vertexCount, bool useNormal, bool useTexture);

    void createElementBufferObject(unsigned int* indexArray, std::size_t indexCount);

    void createTexture(const char* imageFile);

    void setVertexAttribute(bool useNormal, bool useTexture);

    bool hasNormal() const;
    bool hasTexture() const;

    void draw();

    static int oneVertexFloatCount(bool useNormal, bool useTexture);

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
    unsigned int m_texture = 0;
    std::size_t m_elementCount = 0;
    std::size_t m_vertexCount = 0;
    bool m_useNormal = false;
    bool m_useTexture = false;
};
