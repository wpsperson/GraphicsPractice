#include "Performance/CirclesColorAttrib.h"

#include <cmath>
#include <vector>
#include "Core/DefineStruct.h"

#include "Core/OpenGLHeader.h"
#include "Core/DefineConst.h"


struct Point2D {
    float x, y;
};

struct Circle {
    Point2D position;
    float radius;
    int color_index;
};

static const int NUM_CIRCLES_X = 100;
static const int NUM_CIRCLES_Y = 100;
static const int NUM_CIRCLES = NUM_CIRCLES_X * NUM_CIRCLES_Y;
static const int NUM_SEGMENTS = 16; // circle tessellation segments

std::vector<Circle> all_circles;
std::vector<float> vertex_buffer;
std::vector<unsigned int> index_buffer;

GLuint vao, vbo, ebo, shader_program;

// 10 color palette
float color_table[10][3] = {
    {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 1.0},
    {0.0, 1.0, 1.0}, {1.0, 0.5, 0.0}, {0.5, 0.0, 1.0}, {0.0, 0.5, 1.0}, {0.5, 0.5, 0.5}
};

const char* vertex_shader_src = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
out vec3 vColor;
void main() {
    vColor = aColor;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* fragment_shader_src = R"(
#version 330 core
in vec3 vColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vColor, 1.0);
}
)";

GLuint compile_shader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}

void init_shader() {
    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);
    glDeleteShader(vs);
    glDeleteShader(fs);
}

void init_buffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, NUM_CIRCLES * (NUM_SEGMENTS + 1) * 5 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, NUM_CIRCLES * NUM_SEGMENTS * 3 * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
}

void regenerate_circles()
{
    all_circles.clear();
    float spacing = 2.0f / NUM_CIRCLES_X;
    for (int y = 0; y < NUM_CIRCLES_Y; ++y) {
        for (int x = 0; x < NUM_CIRCLES_X; ++x) {
            Circle c;
            c.position = { -1.0f + x * spacing + spacing * 0.5f, -1.0f + y * spacing + spacing * 0.5f };
            c.radius = spacing * 0.4f; // + 0.01f * std::sin(glfwGetTime() + x + y);
            c.color_index = (x + y) % 10;
            all_circles.push_back(c);
        }
    }
}

void build_vbo() {
    vertex_buffer.clear();
    index_buffer.clear();
    unsigned int base_idx = 0;
    for (Circle circle : all_circles)
    {
        auto& color = color_table[circle.color_index];
        vertex_buffer.push_back(circle.position.x);
        vertex_buffer.push_back(circle.position.y);
        vertex_buffer.push_back(color[0]);
        vertex_buffer.push_back(color[1]);
        vertex_buffer.push_back(color[2]);

        for (int i = 0; i < NUM_SEGMENTS; ++i) 
        {
            float angle = float(2.0f * kMathPId * i) / NUM_SEGMENTS;
            float px = circle.position.x + std::cos(angle) * circle.radius;
            float py = circle.position.y + std::sin(angle) * circle.radius;
            vertex_buffer.push_back(px);
            vertex_buffer.push_back(py);
            vertex_buffer.push_back(color[0]);
            vertex_buffer.push_back(color[1]);
            vertex_buffer.push_back(color[2]);
        }

        for (int i = 0; i < NUM_SEGMENTS; ++i) {
            index_buffer.push_back(base_idx);
            index_buffer.push_back(base_idx + i + 1);
            index_buffer.push_back(base_idx + ((i + 1) % NUM_SEGMENTS) + 1);
        }
        base_idx += NUM_SEGMENTS + 1;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_buffer.size() * sizeof(float), vertex_buffer.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, index_buffer.size() * sizeof(unsigned int), index_buffer.data());
}

void draw_scene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, int(index_buffer.size()), GL_UNSIGNED_INT, 0);
}

CirclesColorAttrib::~CirclesColorAttrib()
{

}

void CirclesColorAttrib::initialize(Renderer* renderer) noexcept
{
    init_shader();
    init_buffers();
}

void CirclesColorAttrib::paint(Renderer* renderer) noexcept
{
    regenerate_circles();
    build_vbo();
    draw_scene();
}


