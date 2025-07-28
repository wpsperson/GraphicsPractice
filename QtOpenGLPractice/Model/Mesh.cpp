#include "Model/Mesh.h"

const std::vector<ColorVertex>& ColorMesh::getVertices() const
{
    return vertices;
}

const std::vector<unsigned int>& ColorMesh::getIndices() const
{
    return indices;
}

std::vector<ColorVertex>& ColorMesh::verticesReference()
{
    return vertices;
}

std::vector<unsigned int>& ColorMesh::indicesReference()
{
    return indices;
}

void ColorMesh::mergeMesh(const ColorMesh& mesh)
{
    unsigned int base = unsigned int(vertices.size());

    vertices.reserve(vertices.size() + mesh.vertices.size());
    for (const ColorVertex& vertex : mesh.vertices)
    {
        vertices.emplace_back(vertex);
    }

    for (unsigned int idx : mesh.indices)
    {
        indices.emplace_back(base + idx);
    }
}

void ColorMesh::mergeMeshWithColor(const ColorMesh& mesh, const Color4uc& color)
{
    unsigned int base = unsigned int(vertices.size());

    vertices.reserve(vertices.size() + mesh.vertices.size());
    for (const ColorVertex& vertex : mesh.vertices)
    {
        vertices.emplace_back(vertex.point, color);
    }

    indices.reserve(indices.size() + mesh.indices.size());
    for (unsigned int idx : mesh.indices)
    {
        indices.emplace_back(base + idx);
    }
}

void ColorMesh::setMeshColor(const Color4uc& color)
{
    for (ColorVertex& vertex : vertices)
    {
        vertex.color = color;
    }
}

void ColorMesh::resetMesh()
{
    vertices.clear();
    indices.clear();
}
