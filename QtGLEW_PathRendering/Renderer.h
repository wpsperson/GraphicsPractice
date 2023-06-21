#pragma once

class Renderer
{
public:
    void init();

    void resize(int width, int height);

    void render();
private:
    unsigned int path_object_ = 42;
};