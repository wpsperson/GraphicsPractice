#pragma once

#include <string>
#include <vector>
#include "Box.h"

class ModelData;
struct aiMesh;
struct aiScene;

class AssimpAdaptor
{
public:
    bool load3DModel(const std::string &fileName, std::vector<ModelData*>&output, std::string &errorMsg);

private:
    ModelData* createModelDataByMesh(aiMesh* pMesh);

private:
    std::string m_directory;
    const aiScene* m_scene = nullptr;
    Box m_box;
};
