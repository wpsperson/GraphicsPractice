#pragma once

#include <vector>

#include "OpenGLHeader.h"

class MemorySegment
{
public:
    void setIndex(int idx);

    std::size_t globalVertexOffset();

    std::size_t globalIndexOffset();

    std::size_t globalDrawIndexOffset();

    std::size_t drawElementCount();

    void resetOffset();

    bool hasSpace(std::size_t vert_count, std::size_t indice_count);

    void advanceFillOffset(std::size_t vert_count, std::size_t indice_count);

    void advanceDrawOffset();

    void setSyncPoint();

    void waitSync();

public:
    static void setSegmentCapacityKB(int kb_size);

    static std::size_t SegmentVertexCapacity();

    static std::size_t SegmentIndiceCapacity();

private:
    int segment_index = 0;
    std::size_t vertex_fill_offset = 0;  // offset in segment.
    std::size_t indice_draw_offset = 0;  // offset in segment.
    std::size_t indice_fill_offset = 0;  // offset in segment.
    GLsync sync = nullptr;
    static inline std::size_t kSegmentVertexNum = 256 * 1024;
    static inline std::size_t kSegmentIndiceNum = 256 * 1024;
};