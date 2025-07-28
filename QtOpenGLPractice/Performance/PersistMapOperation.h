#pragma once

#include "Operations/Operation.h"

#include <array>

#include "OpenGLHeader.h"
#include "Const.h"
#include "Model/ManyCircles.h"
#include "Model/Mesh.h"

constexpr std::size_t kSEG_NUM = 3;

struct MemorySegment
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

class PersistMapOperation : public Operation
{
public:
    PersistMapOperation();

    ~PersistMapOperation();

    void initialize(Renderer* renderer) noexcept override;

    void paint(Renderer* renderer) noexcept override;

    void processMesh(const ColorMesh &mesh);

    void drawCurrentSegmentBuffer();

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
    ColorVertex *m_perMapVertices = nullptr;
    unsigned int* m_perMapIndices = nullptr;
    std::array<MemorySegment, kSEG_NUM> m_segments;
    int m_cur_seg_idx = 0;
    ManyCircles m_circles;
    ColorMesh m_tempMesh;
};




