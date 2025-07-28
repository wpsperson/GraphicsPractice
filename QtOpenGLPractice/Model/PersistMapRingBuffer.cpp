#include "Model/PersistMapRingBuffer.h"

#include <algorithm>

PersistMapRingBuffer::PersistMapRingBuffer()
{
}

PersistMapRingBuffer::~PersistMapRingBuffer()
{
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glDeleteBuffers(1, &m_ebo);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void PersistMapRingBuffer::initBuffer()
{
    std::size_t kTotalVBOSize = MemorySegment::SegmentVertexCapacity() * kSEG_NUM * sizeof(ColorVertex);
    std::size_t kTotalEBOSize = MemorySegment::SegmentIndiceCapacity() * kSEG_NUM * sizeof(unsigned int);

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


    for (int idx = 0; idx < kSEG_NUM; idx++)
    {
        MemorySegment& seg = m_segments[idx];
        seg.setIndex(idx);
    }
}

void PersistMapRingBuffer::bindVAO()
{
    glBindVertexArray(m_vao);
}

void PersistMapRingBuffer::processMesh(const ColorMesh& mesh)
{
    const std::vector<ColorVertex>& vertices = mesh.getVertices();
    const std::vector<unsigned int>& indices = mesh.getIndices();
    std::size_t vertice_count = vertices.size();
    std::size_t indice_count = indices.size();
    MemorySegment* current = &(m_segments[m_cur_seg_idx]);
    if (!current->hasSpace(vertice_count, indice_count))
    {
        // 1 draw current segment.
        drawCurrentSegmentBuffer();
        current->setSyncPoint();
        current->resetOffset();

        // 2 switch current to next segment.
        m_cur_seg_idx = (m_cur_seg_idx + 1) % 3;
        current = &(m_segments[m_cur_seg_idx]);
        current->waitSync();
    }

    // 1 copy batch vertices
    std::size_t global_vertice_offset = current->globalVertexOffset();
    ColorVertex* dest = m_perMapVertices + global_vertice_offset;
    const ColorVertex* src = vertices.data();
    std::memcpy(dest, src, vertices.size() * sizeof(ColorVertex));
    // 2 copy batch indices with offset;
    std::size_t global_indice_offset = current->globalIndexOffset();
    unsigned int* idx_dest = m_perMapIndices + global_indice_offset;
    unsigned int offset = unsigned int(global_vertice_offset);
    std::transform(indices.begin(), indices.end(), idx_dest,
        [offset](unsigned int index) { return index + offset; });

    current->advanceFillOffset(vertice_count, indice_count);
}

void PersistMapRingBuffer::drawCurrentSegmentBuffer()
{
    MemorySegment* current = &(m_segments[m_cur_seg_idx]);
    GLsizei count = int(current->drawElementCount());
    if (0 == count)
    {
        return;
    }
    std::size_t begin_offset_byte = current->globalDrawIndexOffset() * sizeof(unsigned int);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(begin_offset_byte));

    current->advanceDrawOffset();
}
