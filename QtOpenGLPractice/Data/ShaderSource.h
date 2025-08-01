#pragma once

const char* const kBaseColorVertexSource = R"(
#version 110
attribute vec2 aPos;
uniform vec4 view;
float convert(float start, float end, float value)
{
    return ((value - start) * 2.0) / (end - start) - 1.0;
}
void main()
{
    gl_Position = vec4(convert(view.x, view.y, aPos.x), convert(view.z, view.w, aPos.y), 0.0, 1.0);
}
)";

const char* const kBaseColorFragmentSource = R"(
#version 110
uniform vec4 baseColor;
void main()
{
    gl_FragColor = baseColor;
}
)";

const char* const kFontVertexSource = R"(
#version 110
attribute vec2 aPos;
attribute vec2 aUV;
varying vec2 varyingUV;
uniform vec4 view;

float convert(float start, float end, float value)
{
    return ((value - start) * 2.0) / (end - start) - 1.0;
}

void main()
{
    varyingUV = aUV;
    gl_Position = vec4(convert(view.x, view.y, aPos.x), convert(view.z, view.w, aPos.y), 0.0, 1.0);
}
)";



const char* const kFontFragmentSource = R"(
#version 110
uniform vec4 baseColor;
uniform sampler2D textureUnit;
varying vec2 varyingUV;
void main()
{
    float texValue = texture2D(textureUnit, varyingUV).r;
    if(texValue < 0.5)
    {
       discard;
    }
    gl_FragColor = vec4(baseColor.xyz, texValue * baseColor.w);
}
)";

const char* const kTextureFragmentSource = R"(
#version 110
uniform sampler2D textureUnit;
varying vec2 varyingUV;
void main()
{
    gl_FragColor = texture2D(textureUnit, varyingUV);
}
)";

// new version
const char* const kColorVertexVS = R"(
#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec4 color;
uniform vec4 view;
out vec4 vertexColor;

vec2 transform2d(vec2 pos)
{
    vec2 result;
    result.x = (pos.x - view.x) / (view.y - view.x);
    result.y = (pos.y - view.z) / (view.w - view.z);
    return result * 2.0 - 1.0;
}

void main()
{
    vertexColor = color;
    gl_Position = vec4(transform2d(pos), 0.0, 1.0);
}
)";

const char* const kColorVertexFS = R"(
#version 330 core
in vec4 vertexColor;
void main()
{
    gl_FragColor = vertexColor;
}
)";


const char* const kPolygonStippleFS = R"(
#version 330 core
in vec4 vertexColor;
uniform sampler2D uStippleTexture;

void main()
{
    vec2 coord = mod(gl_FragCoord.xy, 32.0) / 32.0;
    float mask = texture(uStippleTexture, coord).a;
    if (mask < 0.5)
        discard;
    gl_FragColor = vertexColor;
}
)";

const char* const kPolygonStippleArrayFS = R"(
#version 330 core
in vec4 vertexColor;
uniform sampler2DArray uStippleTextureArray;
uniform uint uStippleIndex;

void main()
{
    vec2 coord = mod(gl_FragCoord.xy, 32.0) / 32.0;
    float mask = texture(uStippleTextureArray, vec3(coord, uStippleIndex)).a;
    if (mask < 0.5)
        discard;
    gl_FragColor = vertexColor;
}
)";



const char* const kLineStippleVS = R"(
#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec4 color;
uniform vec4 view;
out vec4 vertexColor;
flat out vec3 startPos;
out vec3 vertPos;

vec2 transform2d(vec2 pos)
{
    vec2 result;
    result.x = (pos.x - view.x) / (view.y - view.x);
    result.y = (pos.y - view.z) / (view.w - view.z);
    return result * 2.0 - 1.0;
}

void main()
{
    vertexColor = color;
    vec4 pos  = vec4(transform2d(pos), 0.0, 1.0);
    gl_Position = pos;
    vertPos = pos.xyz / pos.w;
    startPos = vertPos;
}
)";

const char* const kLineStippleFS = R"(
#version 330 core
in vec4 vertexColor;
flat in vec3 startPos;
in vec3 vertPos;

uniform vec2  u_resolution;
uniform uint  u_pattern;
uniform float u_factor;

void main()
{
    vec2  dir  = (vertPos.xy-startPos.xy) * u_resolution/2.0;
    float dist = length(dir);

    uint bit = uint(round(dist / u_factor)) & 15U;
    if ((u_pattern & (1U<<bit)) == 0U)
        discard; 

    gl_FragColor = vertexColor;
}
)";



const char* const kLineStippleAttributeVS = R"(
#version 330 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in uint stipple;
uniform vec4 view;
out vec4 vertexColor;
flat out uint vertStipple;
flat out vec3 startPos;
out vec3 vertPos;

vec2 transform2d(vec2 pos)
{
    vec2 result;
    result.x = (pos.x - view.x) / (view.y - view.x);
    result.y = (pos.y - view.z) / (view.w - view.z);
    return result * 2.0 - 1.0;
}

void main()
{
    vertexColor = color;
    vertStipple = stipple;
    vec4 pos  = vec4(transform2d(pos), 0.0, 1.0);
    gl_Position = pos;
    vertPos = pos.xyz / pos.w;
    startPos = vertPos;
}
)";

const char* const kLineStippleAttributeFS = R"(
#version 330 core
in vec4 vertexColor;
flat in vec3 startPos;
in vec3 vertPos;
flat in uint vertStipple;

uniform vec2  u_resolution;


void main()
{
    uint u_pattern = vertStipple & 0xFFFFu;
    uint u_factor = vertStipple >> 16;
    vec2  dir  = (vertPos.xy-startPos.xy) * u_resolution/2.0;
    float dist = length(dir);

    uint bit = uint(round(dist / u_factor)) & 15U;
    if ((u_pattern & (1U<<bit)) == 0U)
        discard; 

    gl_FragColor = vertexColor;
}
)";


