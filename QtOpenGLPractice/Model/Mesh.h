#pragma once

#include "Const.h"
#include <vector>

class ColorMesh
{
public:
    const std::vector<ColorVertex>& getVertices() const;

    const std::vector<unsigned int>& getIndices() const;

    std::vector<ColorVertex>& verticesReference();

    std::vector<unsigned int>& indicesReference();

    void mergeMesh(const ColorMesh& mesh);

    void mergeMeshWithColor(const ColorMesh& mesh, const Color4uc& color);

    void setMeshColor(const Color4uc& color);

    void resetMesh();

private:
    std::vector<ColorVertex> vertices;
    std::vector<unsigned int> indices;
};

