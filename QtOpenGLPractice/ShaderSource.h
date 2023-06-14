#pragma once

const char* const kBaseColorVertexSource =
"#version 110 \n"
"attribute vec2 aPos; \n"
"uniform vec4 view; \n"

"float convert(float start, float end, float value)\n"
"{\n"
"    return ((value - start) * 2.0f) / (end - start) - 1.0f;\n"
"}\n"

"void main() \n"
"{ \n"
"    gl_Position = vec4(convert(view.x, view.y, aPos.x), convert(view.z, view.w, aPos.y), 0.0, 1.0); \n"
"} \n";

const char* const kBaseColorFragmentSource =
"#version 110 \n"
"uniform vec4 baseColor; \n"
"void main() \n"
"{ \n"
"    gl_FragColor = baseColor; \n"
"} \n";


const char* const kFontVertexSource =
"#version 110 \n"
"attribute vec2 aPos; \n"
"attribute vec2 aUV; \n"
"varying vec2 varyingUV; \n"
"uniform vec4 view; \n"

"float convert(float start, float end, float value)\n"
"{\n"
"    return ((value - start) * 2.0f) / (end - start) - 1.0f;\n"
"}\n"

"void main() \n"
"{ \n"
"    varyingUV = aUV; \n"
"    gl_Position = vec4(convert(view.x, view.y, aPos.x), convert(view.z, view.w, aPos.y), 0.0, 1.0); \n"
"} \n";


const char* const kFontFragmentSource =
"#version 110 \n"
"uniform vec4 baseColor; \n"
"uniform sampler2D textureUnit; \n"
"varying vec2 varyingUV; \n"
"void main() \n"
"{ \n"
"    float texValue = texture2D(textureUnit, varyingUV).r; \n"
"    if(texValue < 0.5)\n"
"    {\n"
"       discard;\n"
"    }\n"
"    gl_FragColor = vec4(baseColor.xyz, texValue * baseColor.w); \n"
"} \n";

