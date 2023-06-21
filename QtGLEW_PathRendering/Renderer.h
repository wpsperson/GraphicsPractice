#pragma once

class Renderer
{
public:
    void init();

    void resize(int width, int height);

    void render();
private:
    unsigned int path_object_ = 42;
    //unsigned int glyph_base_ = 0;
    unsigned int font_base_ = 0;
};