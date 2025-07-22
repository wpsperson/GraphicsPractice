#pragma once

#include "Operations/Operation.h"
#include "OpenGLHeader.h"
#include "Const.h"
#include "Model/ManyCircles.h"
#include "Model/Mesh.h"



class PersistMapOperation : public Operation
{
public:
    PersistMapOperation();

    ~PersistMapOperation();

    void initialize(Renderer* renderer) noexcept override;

    void paint(Renderer* renderer) noexcept override;

    void drawAllBuffer();

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
    ColorVertex *m_perMapVertices = nullptr;
    unsigned int* m_perMapIndices = nullptr;
    unsigned int m_verticeIdx = 0;
    unsigned int m_indiceIdx = 0;
    GLsync m_sync = nullptr;
    ManyCircles m_circles;
    ColorMesh m_tempMesh;
};




