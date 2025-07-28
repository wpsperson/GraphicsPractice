#pragma once

#include <array>

#include "Const.h"
#include "Model/SegmentMemory.h"
#include "Model/Mesh.h"

constexpr std::size_t kSEG_NUM = 3;

class PersistMapRingBuffer
{
public:
    PersistMapRingBuffer();

    ~PersistMapRingBuffer();

    void initBuffer();

    void bindVAO();

    void processMesh(const ColorMesh& mesh);

    void drawCurrentSegmentBuffer();


private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
    ColorVertex* m_perMapVertices = nullptr;
    unsigned int* m_perMapIndices = nullptr;
    std::array<MemorySegment, kSEG_NUM> m_segments;
    int m_cur_seg_idx = 0;
};
