#include "Renderer.h"

#include <iostream>
#include <exception>
#define GLEW_STATIC
#include <GL/glew.h>


bool g_useDrawArray = true;

float g_triangleVertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};

float g_quadVertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

unsigned int g_quadIndeces[] = {
    0, 1, 3,
    1, 2, 3
};

const char* vertexShaderSource =
"#version 110 \n"
"attribute vec3 aPos; \n"
"void main() \n"
"{ \n"
"    gl_Position = vec4(aPos, 1.0); \n"
"} \n";

const char* fragmentShaderSource =
"#version 110 \n"
"uniform vec4 baseColor; \n"
"void main() \n"
"{ \n"
"    gl_FragColor = baseColor; \n"
"} \n";


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
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VAO;
    unsigned int VBO;
    unsigned int VEO;
    if (g_useDrawArray)
    {
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_triangleVertices), g_triangleVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }
    else
    {
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &VEO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_quadVertices), g_quadVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_quadIndeces), g_quadIndeces, GL_STATIC_DRAW);
    }

    int nPosBaseColor = glGetUniformLocation(shaderProgram, "baseColor");
    glUniform4f(nPosBaseColor, 0.6f, 0.6f, 1.0f, 1.0f);

}

void Renderer::resize(int width, int height)
{
}

void Renderer::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    if (g_useDrawArray)
    {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    }
}
