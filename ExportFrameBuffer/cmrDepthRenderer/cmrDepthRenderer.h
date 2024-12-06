#pragma once

#include <array>
#include <string>
#include <vector>
#include <tuple>
#include "ProgramManager.h"
#include "LibExport.h"

class GLObject;

class DEPTHR_ENDERER_EXPORT cmrDepthRenderer
{
public:
    cmrDepthRenderer(const std::string &modelURL);
    ~cmrDepthRenderer();

    float* getDepthData(double pose[16], int imageWidth, int imageHeight, double intrinsics[4], double zNear, double zFar, bool useReverseZ);

    std::tuple<float*, unsigned char*, unsigned char *> getDepthColorData(double pose[16], int imageWidth, int imageHeight, double intrinsics[4], double zNear, double zFar, bool useReverseZ);

private:
    bool initOpenGLContext(std::string& errorMsg);

    bool load3dModel(const std::string &fileName, std::string& errorMsg);

    void createFBO(int width, int height, bool useColorBuffer);

    std::array<float, 16> computeModelMatrix(double pose[16]);

    std::array<float, 16> computeProjectionMatrix(int width, int height, double intrinsics[4], double zNear, double zFar, bool useReverseZ);

    void render(int width, int height, const std::array<float, 16>& model, const std::array<float, 16>& projection, bool useReverseZ);

    float* readDepthBuffer(int width, int height);

    unsigned char* readNormalBuffer(int width, int height);

    unsigned char* readColorBuffer(int width, int height);

    void freeGLObjects();

private:
    unsigned int m_frameBuffer = 0;
    unsigned int m_depthRenderBuffer = 0;
    unsigned int m_normalRenderBuffer = 0;
    unsigned int m_colorRenderBuffer = 0;
    ProgramManager m_programMgr;
    GLObject* m_globject = nullptr;
    std::vector<GLObject*> m_objects;
};