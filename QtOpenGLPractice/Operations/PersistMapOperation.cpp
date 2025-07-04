#include "Operations/PersistMapOperation.h"


static const int kMaxVertex = 3 * 1024 * 1024;
static const int kTotalSizeBytes = kMaxVertex * sizeof(Point);

PersistMapOperation::PersistMapOperation()
{
}

PersistMapOperation::~PersistMapOperation()
{
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void PersistMapOperation::initialize(Renderer* renderer) noexcept
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    GLbitfield mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
    GLbitfield createFlags = mapFlags | GL_MAP_DYNAMIC_STORAGE_BIT;
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferStorage(GL_ARRAY_BUFFER, kTotalSizeBytes, nullptr, createFlags);
    m_persistBuffer = reinterpret_cast<Point*> (glMapBufferRange(GL_ARRAY_BUFFER, 0, kTotalSizeBytes, mapFlags));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
    glEnableVertexAttribArray(0);
}

void PersistMapOperation::paint(Renderer* renderer) noexcept
{
}
