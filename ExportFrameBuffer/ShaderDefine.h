#pragma once

const char* const strVertexBasic =
"#version 460 core \n"
"attribute vec3 aPos; \n"
"void main() \n"
"{ \n"
"    gl_Position = vec4(aPos, 1.0); \n"
"} \n";

const char* const strVertex =
"#version 460 core \n"
"layout(location = 0) in vec3 aPos; \n"
"uniform mat4 model; \n"
"uniform mat4 view; \n"
"uniform mat4 projection; \n"
"void main() \n"
"{ \n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0); \n"
"} \n";

const char* const strFragment =
"#version 460 core \n"
"uniform vec4 baseColor; \n"
"void main() \n"
"{ \n"
"    gl_FragColor = baseColor; \n"
"} \n";



const char* const strVertexForNormal =
"#version 460 core \n"
"layout(location = 0) in vec3 aPos; \n"
"layout(location = 1) in vec3 aNormal; \n"
"uniform mat4 model; \n"
"uniform mat4 view; \n"
"uniform mat4 projection; \n"
"out vec3 Normal; \n"
"out vec3 FragPos; \n"
"void main() \n"
"{ \n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0); \n"
"    Normal = mat3(transpose(inverse(model))) * aNormal; \n"
"    FragPos = vec3(model * vec4(aPos, 1.0)); \n"
"} \n";

const char* const strFragmentForNormal =
"#version 460 core \n"
"out vec4 FragColor; \n"
"in vec3 Normal; \n"
"in vec3 FragPos; \n"
"uniform vec3 viewPos; \n"
"uniform vec3 lightDir; \n"
"uniform vec4 baseColor; \n"
"void main() \n"
"{ \n"
"    vec3 ambient = vec3(baseColor) * 0.2; \n"
"    vec3 norm = normalize(Normal); \n"
"    float diff = max(dot(norm, lightDir), 0.0); \n"
"    vec3 diffuse = diff * vec3(baseColor) * 0.5; \n"
"    vec3 viewDir = normalize(viewPos - FragPos); \n"
"    vec3 reflectDir = reflect(-lightDir, norm); \n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); \n"
"    vec3 specular = (spec * vec3(1.0, 1.0, 1.0)) * 0.3; \n"
"    vec3 result = ambient + diffuse + specular; \n"
"    FragColor = vec4(result, 1.0); \n"
"} \n";


const char* const strVertexForTexture =
"#version 460 core \n"
"layout(location = 0) in vec3 aPos; \n"
"layout(location = 2) in vec2 aUV; \n"
"uniform mat4 model; \n"
"uniform mat4 view; \n"
"uniform mat4 projection; \n"
"out vec2 uvCoord; \n"
"void main() \n"
"{ \n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0); \n"
"    uvCoord = aUV; \n"
"} \n";

const char* const strFragmentForTexture =
"#version 460 core \n"
"out vec4 FragColor; \n"
"in vec2 uvCoord; \n"
"uniform sampler2D diffuseTexture; \n"
"void main() \n"
"{ \n"
"    FragColor = texture(diffuseTexture, uvCoord); \n"
"} \n";



const char* const strVertexForNormalTexture =
"#version 460 core \n"
"layout(location = 0) in vec3 aPos; \n"
"layout(location = 1) in vec3 aNormal; \n"
"layout(location = 2) in vec2 aUV; \n"
"uniform mat4 model; \n"
"uniform mat4 view; \n"
"uniform mat4 projection; \n"
"out vec2 uvCoord; \n"
"out vec3 Normal; \n"
"out vec3 FragPos; \n"
"void main() \n"
"{ \n"
"    gl_Position = projection * view * model * vec4(aPos, 1.0); \n"
"    uvCoord = aUV; \n"
"    Normal = mat3(transpose(inverse(model))) * aNormal; \n"
"    FragPos = vec3(model * vec4(aPos, 1.0)); \n"
"} \n";

const char* const strFragmentForNormalTexture =
"#version 460 core \n"
"out vec4 FragColor; \n"
"in vec3 Normal; \n"
"in vec3 FragPos; \n"
"in vec2 uvCoord; \n"
"uniform sampler2D diffuseTexture; \n"
"uniform vec3 viewPos; \n"
"uniform vec3 lightDir; \n"
"uniform vec4 baseColor; \n"
"void main() \n"
"{ \n"
"    vec3 ambient = vec3(baseColor) * 0.2; \n"
"    vec3 norm = normalize(Normal); \n"
"    float diff = max(dot(norm, lightDir), 0.0); \n"
"    vec3 diffuse = diff * vec3(baseColor) * 0.5; \n"
"    vec3 viewDir = normalize(viewPos - FragPos); \n"
"    vec3 reflectDir = reflect(-lightDir, norm); \n"
"    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); \n"
"    vec3 specular = (spec * vec3(1.0, 1.0, 1.0)) * 0.3; \n"
"    vec3 ads_component = ambient + diffuse + specular; \n"
"    vec4 texture_component = texture(diffuseTexture, uvCoord); \n"
"    FragColor = vec4(ads_component, 1.0) * 0.2 + texture_component * 0.8; \n"
"} \n";








