#include "GLObject.h"

#include <cassert>

#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static constexpr int VertexLayoutIndex = 0;
static constexpr int NormalLayoutIndex = 1;
static constexpr int TextureLayoutIndex = 2;


GLObject::GLObject()
{
}

GLObject::~GLObject()
{
    if (m_ebo)
    {
        glDeleteBuffers(1, &m_ebo);
    }
    if (m_vbo)
    {
        glDeleteBuffers(1, &m_vbo);
    }
    if (m_vao)
    {
        glDeleteVertexArrays(1, &m_vao);
    }
}

void GLObject::createVertexArrayObject()
{
    glGenVertexArrays(1, &m_vao);
}

void GLObject::createVertexBufferObject(float* vertexArray, std::size_t vertexCount, bool useNormal, bool useTexture)
{
    assert(m_vao > 0);
    glBindVertexArray(m_vao);

    m_useNormal = useNormal;
    m_useTexture = useTexture;
    m_vertexCount = vertexCount;
    std::size_t bufferSize = vertexCount * oneVertexFloatCount(useNormal, useTexture) * sizeof(float);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, vertexArray, GL_STATIC_DRAW);

    setVertexAttribute(useNormal, useTexture);
}

void GLObject::createElementBufferObject(unsigned int* indexArray, std::size_t indexCount)
{
    assert(m_vao > 0);
    glBindVertexArray(m_vao);

    m_elementCount = indexCount;
    std::size_t bufferSize = indexCount * sizeof(unsigned int);
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, indexArray, GL_STATIC_DRAW);
}

void GLObject::createTexture(const char* imageFile)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(imageFile, &width, &height, &nrChannels, 0);
    if (!data)
    {
        return;
    }

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void GLObject::setVertexAttribute(bool useNormal, bool useTexture)
{
    assert(m_vbo > 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    int stride = oneVertexFloatCount(useNormal, useTexture) * sizeof(float);
    int offset = 0;
    glEnableVertexAttribArray(VertexLayoutIndex);
    glVertexAttribPointer(VertexLayoutIndex, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    offset += 3 * sizeof(float);

    if (useNormal)
    {
        glEnableVertexAttribArray(NormalLayoutIndex);
        glVertexAttribPointer(NormalLayoutIndex, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 3 * sizeof(float);
    }
    if (useTexture)
    {
        glEnableVertexAttribArray(TextureLayoutIndex);
        glVertexAttribPointer(TextureLayoutIndex, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 2 * sizeof(float);
    }
}

bool GLObject::hasNormal() const
{
    return m_useNormal;
}

bool GLObject::hasTexture() const
{
    return m_useTexture;
}

void GLObject::draw()
{
    if (!m_vao)
    {
        return;
    }
    assert((m_texture > 0) == m_useTexture);

    if (m_texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }
    glBindVertexArray(m_vao);
    if (m_ebo)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_elementCount), GL_UNSIGNED_INT, (void*)0);
    }
    else if (m_vbo)
    {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertexCount));
    }
}

int GLObject::oneVertexFloatCount(bool useNormal, bool useTexture)
{
    return useNormal ? (useTexture ? 8 : 6) : (useTexture ? 5 : 3);
}
