#pragma once

#include <vector>

#include "Core/DefineStruct.h"

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

class ColorStipMesh
{
public:
    const std::vector<ColorStipVertex>& getVertices() const;

    const std::vector<unsigned int>& getIndices() const;

    std::vector<ColorStipVertex>& verticesReference();

    std::vector<unsigned int>& indicesReference();

    void resetMesh();

private:
    std::vector<ColorStipVertex> vertices;
    std::vector<unsigned int> indices;
};