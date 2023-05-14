#include "cmrDepthRenderer.h"

#include <corecrt_math_defines.h>
#include <vector>
#include <iostream>
#include <exception>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "GLObject.h"

static const int g_OpenGLVersionMajor = 4;
static const int g_OpenGLVersionMinor = 6;

unsigned int g_quadIndeces[] = {
    0, 1, 2,
    0, 2, 3
};

using namespace std;

cmrDepthRenderer::cmrDepthRenderer(const std::string& modelURL)
{
    std::string errorMsg;
    if (!initOpenGLContext(errorMsg))
    {
        std::cout << "cmrDepthRenderer failde to init opengl context: " << errorMsg << std::endl;
    }
    if (!load3dModel(modelURL, errorMsg))
    {
        std::cout << "cmrDepthRenderer failde to load the 3d model: " << errorMsg << std::endl;
    }
}

cmrDepthRenderer::~cmrDepthRenderer()
{
    if (m_globject)
    {
        delete m_globject;
    }

    if (m_depthRenderBuffer)
    {
        glDeleteRenderbuffers(1, &m_depthRenderBuffer);
    }
    if (m_colorRenderBuffer)
    {
        glDeleteRenderbuffers(1, &m_colorRenderBuffer);
    }
    if (m_frameBuffer)
    {
        glDeleteFramebuffers(1, &m_frameBuffer);
    }
}

float* cmrDepthRenderer::getDepthData(double pose[16], int imageWidth, int imageHeight, double intrinsics[4], double zNear, double zFar, bool useReverseZ)
{
    createFBO(imageWidth, imageHeight, false);
    std::array<float, 16> model = computeModelMatrix(pose);
    std::array<float, 16> projection = computeProjectionMatrix(imageWidth, imageHeight, intrinsics, zNear, zFar, useReverseZ);
    render(imageWidth, imageHeight, model, projection, useReverseZ);
    float *depthBuffer = readDepthBuffer(imageWidth, imageHeight);
    return depthBuffer;
}

std::tuple<float*, unsigned char*> cmrDepthRenderer::getDepthColorData(double pose[16], int imageWidth, int imageHeight, double intrinsics[4], double zNear, double zFar, bool useReverseZ)
{
    createFBO(imageWidth, imageHeight, true);
    std::array<float, 16> model = computeModelMatrix(pose);
    std::array<float, 16> projection = computeProjectionMatrix(imageWidth, imageHeight, intrinsics, zNear, zFar, useReverseZ);
    render(imageWidth, imageHeight, model, projection, useReverseZ);
    float* depthBuffer = readDepthBuffer(imageWidth, imageHeight);
    unsigned char* colorBuffer = readColorBuffer(imageWidth, imageHeight);
    return std::tuple<float*, unsigned char*>(depthBuffer, colorBuffer);
}

bool cmrDepthRenderer::initOpenGLContext(std::string& errorMsg)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, g_OpenGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, g_OpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    int windowWidth = 4096;
    int windwoHeight = 4096;
    GLFWwindow* window = glfwCreateWindow(windowWidth, windwoHeight, "virtual window", NULL, NULL);
    if (!window)
    {
        errorMsg = "Failed to create GLFW window!";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL())
    {
        errorMsg = "Failed to load glad!";
        return false;
    }
    return true;
}

bool cmrDepthRenderer::load3dModel(const std::string& fileName, std::string& errorMsg)
{
    int debugFlag = 0;
    if (0 == debugFlag)
    {
        float quadVertice[] = {
            -0.5f, -1.0f, -1.0f,
              0.5f, -1.0f, -1.0f,
              1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
        };
        m_globject = new GLObject;
        m_globject->createVertexArrayObject();
        m_globject->createVertexBufferObject(quadVertice, 4, false, false);
        m_globject->createElementBufferObject(g_quadIndeces, 6);
    }
    else if (1 == debugFlag)
    {
        float quadVerticeUV[] = {
        -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        };
        m_globject = new GLObject;
        m_globject->createVertexArrayObject();
        m_globject->createVertexBufferObject(quadVerticeUV, 4, false, true);
        m_globject->createElementBufferObject(g_quadIndeces, 6);
        m_globject->createTexture("D:/WorkSpace/cube.png");
    }
    else if (2 == debugFlag) // vertex + normal
    {
        float quadVerticeNormal[] = {
        -0.5f, -0.5f, -1.0f,    0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 1.0f,      0.707f, 0.0f,0.707f,
        0.5f, 0.5f, 0.0f,       0.0f, 0.707f,0.707f,
        -0.5f, 0.5f, 0.0f,      0.0f, 0.707f,0.707f,
        };
        m_globject = new GLObject;
        m_globject->createVertexArrayObject();
        m_globject->createVertexBufferObject(quadVerticeNormal, 4, true, false);
        m_globject->createElementBufferObject(g_quadIndeces, 6);
    }
    else if (3 == debugFlag) // vertex + normal
    {
        float quadVerticeNormal[] = {
        -0.5f, -0.5f, -1.0f,    0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
        0.5f, -0.5f, 1.0f,      0.707f, 0.0f,0.707f,    1.0f, 0.0f,
        0.5f, 0.5f, 0.0f,       0.0f, 0.707f,0.707f,    1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,      0.0f, 0.707f,0.707f,    0.0f, 1.0f,
        };
        m_globject = new GLObject;
        m_globject->createVertexArrayObject();
        m_globject->createVertexBufferObject(quadVerticeNormal, 4, true, true);
        m_globject->createElementBufferObject(g_quadIndeces, 6);
        m_globject->createTexture("D:/WorkSpace/cube.png");
    }
    return true;
}

void cmrDepthRenderer::createFBO(int width, int height, bool useColorBuffer)
{
    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_DRAW_BUFFER, m_frameBuffer);

    glGenRenderbuffers(1, &m_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
    glFramebufferRenderbuffer(GL_DRAW_BUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);

    if (useColorBuffer)
    {
        glGenRenderbuffers(1, &m_colorRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_colorRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_BGRA, width, height);
        glFramebufferRenderbuffer(GL_DRAW_BUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorRenderBuffer);
    }
}

std::array<float, 16> cmrDepthRenderer::computeModelMatrix(double pose[16])
{
    // make it column-major
    std::array<float, 16> modelMatrix = { 
        static_cast<float>(pose[0]), static_cast<float>(pose[4]), static_cast<float>(pose[8]), static_cast<float>(pose[12]), 
        static_cast<float>(pose[1]), static_cast<float>(pose[5]), static_cast<float>(pose[9]), static_cast<float>(pose[13]),
        static_cast<float>(pose[2]), static_cast<float>(pose[6]), static_cast<float>(pose[10]),static_cast<float>(pose[14]),
        static_cast<float>(pose[3]), static_cast<float>(pose[7]), static_cast<float>(pose[11]),static_cast<float>(pose[15])
    };
    return modelMatrix;
}

std::array<float, 16> cmrDepthRenderer::computeProjectionMatrix(int width, int height, double intrinsics[4], double n, double f, bool useReverseZ)
{
    double ax = intrinsics[0];
    double ay = intrinsics[1];
    double u0 = intrinsics[2];
    double v0 = intrinsics[3];

    float m00 = static_cast<float>(2 * ax / width);
    float m11 = static_cast<float>(2 * ay / height);
    float m02 = static_cast<float>(1 - 2 * u0 / width);
    float m12 = static_cast<float>(2 * v0 / height - 1);
    float A = static_cast<float>(-(f + n) / (f - n));
    float B = static_cast<float>(-2 * f * n / (f - n));
    if (useReverseZ)
    {
        float A = static_cast<float>(n / (f - n));
        float B = static_cast<float>(f * n / (f - n));
        std::array<float, 16> projectionMatrix = {
        m00,    0.0f,   0.0f,   0.0f,
        0.0f,   m11,    0.0f,   0.0f,
        m02,    m12,    A,      -1,
        0.0f,   0.0f,   B,      0.0f
        };
        return projectionMatrix;
    }
    else
    {
        std::array<float, 16> projectionMatrix = {
        m00,    0.0f,   0.0f,   0.0f,
        0.0f,   m11,    0.0f,   0.0f,
        m02,    m12,    A,      -1,
        0.0f,   0.0f,   B,      0.0f
        };
        return projectionMatrix;
    }
}

void cmrDepthRenderer::render(int width, int height, const std::array<float, 16>& model, const std::array<float, 16>& projection, bool useReverseZ)
{
    glBindFramebuffer(GL_DRAW_BUFFER, m_frameBuffer);
    // setup window coordinate
    glViewport(0, 0, width, height);
    glDepthRangef(0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    if (useReverseZ)
    {
        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
        glClearDepth(0.0);
        glDepthFunc(GL_GREATER);
    }
    else
    {
        glClearDepth(1.0);
        glDepthFunc(GL_LESS);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    bool hasNormal = m_globject->hasNormal();
    bool hasTexture = m_globject->hasTexture();
    std::array<float, 16> view = { 1,0,0,0,  0,1,0,0,  0,0,1,0, 0,0,0,1 };
    m_programMgr.applyProgram(hasNormal, hasTexture, model, view, projection);
    m_globject->draw();
}

float* cmrDepthRenderer::readDepthBuffer(int width, int height)
{
    float* depthBuffer = new float[width * height];
    glReadBuffer(GL_DEPTH_ATTACHMENT);
    glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depthBuffer);
    return depthBuffer;
}

unsigned char* cmrDepthRenderer::readColorBuffer(int width, int height)
{
    unsigned char* colorBuffer = new unsigned char[width * height * 4];
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer);
    return colorBuffer;
}



