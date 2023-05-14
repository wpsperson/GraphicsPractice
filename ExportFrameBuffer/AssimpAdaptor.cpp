#include "AssimpAdaptor.h"

#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ModelData.h"
#include "Box.h"

bool AssimpAdaptor::load3DModel(const std::string& fileName, std::vector<ModelData*>& output, std::string& errorMsg)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        errorMsg = "Failed to load " + fileName + ", detail info: " + importer.GetErrorString();
        return false;
    }
    m_scene = scene;
    m_directory = fileName.substr(0, fileName.find_last_of('/'));

    output.clear();
    std::size_t meshCount = scene->mNumMeshes;
    for (std::size_t idx = 0; idx < meshCount;idx++)
    {
        aiMesh* pMesh = scene->mMeshes[idx];
        ModelData* modelData = createModelDataByMesh(pMesh);
        output.emplace_back(modelData);
    }
    return true;
}

ModelData* AssimpAdaptor::createModelDataByMesh(aiMesh* pMesh)
{
    bool useNormal = pMesh->HasNormals();
    bool useTexture = pMesh->HasTextureCoords(0);
    std::string strImageFile;
    if (useTexture)
    {
        // texture
        aiMaterial* material = m_scene->mMaterials[pMesh->mMaterialIndex];
        const std::string strSampleName = "texture_diffuse";
        int nTypeTexCount = material->GetTextureCount(aiTextureType_DIFFUSE);
        if (nTypeTexCount > 0)
        {
            aiString imageName;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &imageName); // get the first diffuse texture
            strImageFile = m_directory + "/" + std::string(imageName.C_Str());
            if (!std::filesystem::exists(strImageFile))
            {
                useTexture = false;
            }
        }
        else
        {
            useTexture = false;
        }
    }


    ModelData* model = new ModelData(useNormal, useTexture);
    if (useTexture)
    {
        model->setImageFile(strImageFile);
    }
    model->setVertexNum(pMesh->mNumVertices);

    for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
    {
        if (useNormal && useTexture)
        {
            model->addVertexNormalUV(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z,
                pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z,
                pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y
                );
        }
        else if (useNormal)
        {
            model->addVertexNormal(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z,
                pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z);
        }
        else if (useTexture)
        {
            model->addVertexUV(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z,
                pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y);
        }
        else
        {
            model->addVertex(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
        }
        m_box.mergePt(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
    }

    for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
    {
        aiFace face = pMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            model->addIndex(face.mIndices[j]);
        }
    }

    return model;
}
