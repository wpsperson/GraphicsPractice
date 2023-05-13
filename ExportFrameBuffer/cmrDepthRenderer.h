#pragma once

#include <array>
#include <string>
#include <tuple>
#include "ProgramManager.h"

class GLObject;

class cmrDepthRenderer
{
public:
    cmrDepthRenderer(const std::string &modelURL);
    ~cmrDepthRenderer();

    float* getDepthData(double pose[16], int imageWidth, int imageHeight, double intrinsics[4], double zNear, double zFar, bool useReverseZ);

    std::tuple<float* ,unsigned char *> getDepthColorData(double pose[16], int imageWidth, int imageHeight, double intrinsics[4], double zNear, double zFar, bool useReverseZ);

private:
    bool initOpenGLContext(std::string& errorMsg);

    bool load3dModel(const std::string &fileName, std::string& errorMsg);

    void createFBO(int width, int height, bool useColorBuffer);

    void render(int width, int height, const std::array<float, 16>& model, const std::array<float, 16>& projection, bool useReverseZ);

    float* readDepthBuffer(int width, int height);

    unsigned char* readColorBuffer(int width, int height);

private:
    unsigned int m_frameBuffer = 0;
    unsigned int m_depthRenderBuffer = 0;
    unsigned int m_colorRenderBuffer = 0;
    ProgramManager m_programMgr;
    GLObject* m_globject = nullptr;
};