#pragma once

class Renderer
{
public:
    void init();

    void resize(int width, int height);

    void render();
private:
    unsigned int shaderProgram = 0;
};