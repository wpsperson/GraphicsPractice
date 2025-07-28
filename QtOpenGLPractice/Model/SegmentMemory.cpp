#include "Model/SegmentMemory.h"

#include <chrono>
#include <iostream>


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
    trigger_count++;
    static double total_wait_time = 0.0;


    auto start = std::chrono::high_resolution_clock::now();
    GLenum result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1'000'000);
    while (result != GL_ALREADY_SIGNALED && result != GL_CONDITION_SATISFIED)
    {
        result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1'000'000);
    }
    glDeleteSync(sync);
    sync = nullptr;

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
    total_wait_time += elapsed_ms;

    if (trigger_count == 30)
    {
        double average_wait_time = total_wait_time / trigger_count;
        std::cout << "wait count is (ms): " << average_wait_time << std::endl;
        trigger_count = 0;
        total_wait_time = 0.0;
    }
}

void MemorySegment::setSegmentCapacityKB(int kb_size)
{
    kSegmentVertexNum = kb_size * 1024;
    kSegmentIndiceNum = kb_size * 1024;
}

std::size_t MemorySegment::SegmentVertexCapacity()
{
    return kSegmentVertexNum;
}

std::size_t MemorySegment::SegmentIndiceCapacity()
{
    return kSegmentIndiceNum;
}
