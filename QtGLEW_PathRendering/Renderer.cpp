#include "Renderer.h"

#include <iostream>
#include <exception>
#define GLEW_STATIC
#include <GL/glew.h>

const char* svgPathString =
// star
"M100,180 L40,10 L190,120 L10,120 L160,10 z"
// heart
"M300 300 C 100 400,100 200,300 100,500 200,500 400,300 300Z";

void Renderer::init()
{
    GLenum GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult)
    {
        const char* error_message = reinterpret_cast<const char*>(glewGetErrorString(GlewInitResult));
        if (error_message)
        {
            std::cerr << error_message << std::endl;
        }
        exit(EXIT_FAILURE);
    }

    if (!GLEW_VERSION_2_0)
    {
        throw std::exception("Required OpenGL 2.0 Features are not available!");
    }
    if (!GLEW_NV_path_rendering)
    {
        return;
    }
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xff);
    glStencilFunc(GL_NOTEQUAL, 0, 0x1f);
    glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 500, 0, 400, -1, 1);

    glPathStringNV(path_object_, GL_PATH_FORMAT_SVG_NV, (GLsizei)strlen(svgPathString), svgPathString);
    glPathParameteriNV(path_object_, GL_PATH_JOIN_STYLE_NV, GL_ROUND_NV);
    glPathParameteriNV(path_object_, GL_PATH_STROKE_WIDTH_NV, 6.5);

}

void Renderer::resize(int width, int height)
{
}

void Renderer::render()
{
    glClearStencil(0x00);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (!GLEW_NV_path_rendering)
    {
        return;
    }

    glStencilFillPathNV(path_object_, GL_COUNT_UP_NV, 0x1F);
    glColor3f(1, 1, 0);
    glCoverFillPathNV(path_object_, GL_BOUNDING_BOX_NV);

    glStencilStrokePathNV(path_object_, 0x1, ~0);
    glColor3f(1, 1, 1);
    glCoverStrokePathNV(path_object_, GL_CONVEX_HULL_NV);
}
