#include "Operations/PersistMapOperation.h"

#include <iostream>

#include "Renderer.h"
#include "ProgramManager.h"


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


    for (int idx = 0; idx < kSEG_NUM; idx++)
    {
        MemorySegment& seg = m_segments[idx];
        seg.setIndex(idx);
    }
    m_circles.setCircleNumInBatch(100);
}

void PersistMapOperation::paint(Renderer* renderer) noexcept
{
    m_circles.rebuildRandomInfos();

    ProgramManager*progMgr = renderer->programMgr();
    progMgr->applyProgram(ProgramType::ColorVertex);
    glBindVertexArray(m_vao);

    int batch_count = m_circles.batchCount();
    for (int idx = 0; idx < batch_count; idx++)
    {
        m_tempMesh.resetMesh();
        m_circles.buildBatchMesh(idx, m_tempMesh);
        processMesh(m_tempMesh);
    }
    drawCurrentSegmentBuffer();
}

void PersistMapOperation::processMesh(const ColorMesh& mesh)
{
    std::vector<ColorVertex>& vertices = m_tempMesh.verticesReference();
    std::vector<unsigned int>& indices = m_tempMesh.indicesReference();
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
    ColorVertex* src = vertices.data();
    std::memcpy(dest, src, vertices.size() * sizeof(ColorVertex));
    // 2 copy batch indices with offset;
    std::size_t global_indice_offset = current->globalIndexOffset();
    unsigned int* idx_dest = m_perMapIndices + global_indice_offset;
    unsigned int offset = unsigned int(global_vertice_offset);
    std::transform(indices.begin(), indices.end(), idx_dest,
        [offset](unsigned int index) { return index + offset; });

    current->advanceFillOffset(vertice_count, indice_count);
}

void PersistMapOperation::drawCurrentSegmentBuffer()
{
    MemorySegment* current = &(m_segments[m_cur_seg_idx]);
    GLsizei count = int(current->drawElementCount());
    if (0 == count)
    {
        return;
    }
    std::size_t begin_offset_byte = current->globalDrawIndexOffset() * sizeof(unsigned int);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void *)(begin_offset_byte));

    current->advanceDrawOffset();
}

void MemorySegment::setIndex(int idx)
{
    segment_index = idx;
}

std::size_t MemorySegment::globalVertexOffset()
{
    return segment_index * kSegmentVertexNum + vertex_fill_offset;
}

std::size_t MemorySegment::globalIndexOffset()
{
    return segment_index * kSegmentIndiceNum + indice_fill_offset;
}

std::size_t MemorySegment::globalDrawIndexOffset()
{
    return segment_index * kSegmentIndiceNum + indice_draw_offset;
}

std::size_t MemorySegment::drawElementCount()
{
    return indice_fill_offset - indice_draw_offset;
}

void MemorySegment::resetOffset()
{
    vertex_fill_offset = 0;
    indice_draw_offset = 0;
    indice_fill_offset = 0;
}

bool MemorySegment::hasSpace(std::size_t vert_count, std::size_t indice_count)
{
    if (vertex_fill_offset + vert_count > kSegmentVertexNum
        || indice_fill_offset + indice_count > kSegmentIndiceNum)
    {
        return false;
    }
    return true;
}

void MemorySegment::advanceFillOffset(std::size_t vert_count, std::size_t indice_count)
{
    vertex_fill_offset += vert_count;
    indice_fill_offset += indice_count;
}

void MemorySegment::advanceDrawOffset()
{
    indice_draw_offset = indice_fill_offset;
}

void MemorySegment::setSyncPoint()
{
    sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void MemorySegment::waitSync()
{
    if (!sync)
    {
        return;
    }
    static int trigger_count = 0;
    static int wait_count = 0;
    trigger_count++;
    GLenum result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 10'000);;
    while (result != GL_ALREADY_SIGNALED && result != GL_CONDITION_SATISFIED)
    {
        result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 10'000);
        wait_count++;
    }
    glDeleteSync(sync);
    sync = nullptr;

    if (trigger_count == 100)
    {
        double average_wait_time = 0.01 * double(wait_count) / trigger_count;
        std::cout << "wait count is (ms): " << average_wait_time << std::endl;
        trigger_count = 0;
        wait_count = 0;
    }
}
