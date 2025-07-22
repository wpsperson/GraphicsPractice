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
out vec4 vertexColor;
void main()
{
    vertexColor = color;
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
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