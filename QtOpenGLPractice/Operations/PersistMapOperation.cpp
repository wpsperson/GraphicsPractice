#include "Operations/PersistMapOperation.h"
#include "Renderer.h"
#include "ProgramManager.h"


static constexpr int kMaxVertex = 1024 * 1024;
static constexpr int kTotalVBOSize = kMaxVertex * sizeof(ColorVertex);
static constexpr int kMaxIndiceCount = 1024 * 1024;
static constexpr int kTotalEBOSize = kMaxIndiceCount * sizeof(unsigned int);

PersistMapOperation::PersistMapOperation()
{
}

PersistMapOperation::~PersistMapOperation()
{
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glDeleteBuffers(1, &m_ebo);
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
    glBufferStorage(GL_ARRAY_BUFFER, kTotalVBOSize, nullptr, createFlags);
    m_perMapVertices = reinterpret_cast<ColorVertex*> (glMapBufferRange(GL_ARRAY_BUFFER, 0, kTotalVBOSize, mapFlags));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ColorVertex), (void*)sizeof(Point));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, kTotalEBOSize, nullptr, createFlags);
    m_perMapIndices = reinterpret_cast<unsigned int*>(glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, kTotalEBOSize, mapFlags));


    m_circles.setCircleNumInBatch(100);
    m_circles.rebuildInfos();
}

void PersistMapOperation::paint(Renderer* renderer) noexcept
{
    ProgramManager*progMgr = renderer->programMgr();
    progMgr->applyProgram(ProgramType::ColorVertex);
    glBindVertexArray(m_vao);

    int batch_count = m_circles.batchCount();
    for (int idx = 0; idx < batch_count; idx++)
    {
        m_tempMesh.resetMesh();
        m_circles.buildBatchMesh(idx, m_tempMesh);
        
        std::vector<ColorVertex>& vertices = m_tempMesh.verticesReference();
        std::vector<unsigned int>& indices = m_tempMesh.indicesReference();
        unsigned int vertice_count = unsigned int(vertices.size());
        unsigned int indice_count = unsigned int(indices.size());
        if (m_verticeIdx + vertice_count > kMaxVertex || m_indiceIdx + indice_count > kMaxIndiceCount)
        {
            drawAllBuffer();
        }
        // 1 copy batch vertices
        ColorVertex* dest = m_perMapVertices + m_verticeIdx;
        ColorVertex* src = vertices.data();
        std::memcpy(dest, src, vertices.size() * sizeof(ColorVertex));
        // 2 copy batch indices with offset;
        unsigned int* idx_dest = m_perMapIndices + m_indiceIdx;
        unsigned int offset = m_verticeIdx;
        std::transform(indices.begin(), indices.end(), idx_dest,
            [offset](unsigned int index) { return index + offset; });
        m_verticeIdx += vertice_count;
        m_indiceIdx += indice_count;
    }
    drawAllBuffer();
}

void PersistMapOperation::drawAllBuffer()
{
    int count = int(m_indiceIdx);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    m_verticeIdx = 0;
    m_indiceIdx = 0;
}
