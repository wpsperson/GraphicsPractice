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


const char* const word = "OpenGL";


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

    glPathStringNV(path_object_, GL_PATH_FORMAT_SVG_NV, (GLsizei)strlen(svgPathString), svgPathString);
    glPathParameteriNV(path_object_, GL_PATH_JOIN_STYLE_NV, GL_ROUND_NV);
    glPathParameteriNV(path_object_, GL_PATH_STROKE_WIDTH_NV, 6.5);

    glyph_base_ = glGenPathsNV(6);
    const GLsizei wordLen = (GLsizei)strlen(word);
    const GLfloat emScale = 2048; // match TrueType convention
    GLuint templatePathObject = ~0; // Non-existant path object
    glPathGlyphsNV(glyph_base_, GL_SYSTEM_FONT_NAME_NV, "Helvetica", GL_BOLD_BIT_NV, wordLen, GL_UNSIGNED_BYTE, word, GL_SKIP_MISSING_GLYPH_NV, templatePathObject, emScale);
    glPathGlyphsNV(glyph_base_, GL_SYSTEM_FONT_NAME_NV, "Arial", GL_BOLD_BIT_NV, wordLen, GL_UNSIGNED_BYTE, word, GL_SKIP_MISSING_GLYPH_NV, templatePathObject, emScale);
    glPathGlyphsNV(glyph_base_, GL_STANDARD_FONT_NAME_NV, "Sans", GL_BOLD_BIT_NV, wordLen, GL_UNSIGNED_BYTE, word, GL_USE_MISSING_GLYPH_NV, templatePathObject, emScale);
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 500, 0, 400, -1, 1);
    glPathColorGenNV(GL_PRIMARY_COLOR, GL_NONE, GL_NONE, nullptr);
    glStencilFillPathNV(path_object_, GL_COUNT_UP_NV, 0x1F);
    glColor3f(1, 1, 0);
    glCoverFillPathNV(path_object_, GL_BOUNDING_BOX_NV);
    glStencilStrokePathNV(path_object_, 0x1, ~0);
    glColor3f(1, 1, 1);
    glCoverStrokePathNV(path_object_, GL_CONVEX_HULL_NV);


    GLfloat xtranslate[6 + 1]; // wordLen+1
    xtranslate[0] = 0; // Initial glyph offset is zero
    const GLsizei wordLen = (GLsizei)strlen(word);
     // repeat last letter twice
    glGetPathSpacingNV(GL_ACCUM_ADJACENT_PAIRS_NV, wordLen + 1, GL_UNSIGNED_BYTE, "\000\001\002\003\004\005\005", glyph_base_, 1.0f, 1.0f, GL_TRANSLATE_X_NV, xtranslate + 1);
    GLfloat yMinMax[2];
    glGetPathMetricRangeNV(GL_FONT_Y_MIN_BOUNDS_BIT_NV | GL_FONT_Y_MAX_BOUNDS_BIT_NV, glyph_base_, /*count*/1, 2 * sizeof(GLfloat), yMinMax);
    glMatrixLoadIdentityEXT(GL_PROJECTION);
    glMatrixOrthoEXT(GL_PROJECTION, 0, xtranslate[6], yMinMax[0], yMinMax[1], -1, 1);
    glMatrixLoadIdentityEXT(GL_MODELVIEW);
    const GLfloat rgbGen[3][3] = {
    { 0, 0, 0 }, // red = constant zero
    { 0, 1, 0 }, // green = varies with y from bottom (0) to top (1)
    { 0, -1, 1 } // blue = varies with y from bottom (1) to top (0)
        };
    glPathColorGenNV(GL_PRIMARY_COLOR, GL_PATH_OBJECT_BOUNDING_BOX_NV, GL_RGB, &rgbGen[0][0]);
    glStencilFillPathInstancedNV(6, GL_UNSIGNED_BYTE, "\000\001\002\003\004\005", glyph_base_, GL_PATH_FILL_MODE_NV, 0xFF, GL_TRANSLATE_X_NV, xtranslate);
    glColor3f(0.5, 0.5, 0.5); // 50% gray
    glCoverFillPathInstancedNV(6, GL_UNSIGNED_BYTE, "\000\001\002\003\004\005", glyph_base_, GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV, GL_TRANSLATE_X_NV, xtranslate);

}
