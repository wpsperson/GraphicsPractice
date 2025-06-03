#pragma once

#include <chrono>
#include <string>
#include <vector>
#include "Const.h"

class GLObject2D;
class ProgramManager;
class FontManager;
class ViewPort;
struct Box;

class Renderer
{
public:
    Renderer();

    ~Renderer();

    bool initialize(std::string &err);

    void resize(int width, int height);

    void render();

    void endRender();

    ProgramManager* programMgr() noexcept;

    FontManager* fontMgr() noexcept;

    ViewPort* viewPort() const noexcept;

    void paintObject(GLObject2D* object) noexcept;

    void paintFont(GLObject2D* object) noexcept;

    void legacyProjection() noexcept;

private:
    ProgramManager* m_programMgr = nullptr;
    FontManager* m_fontMgr = nullptr;
    ViewPort* m_viewport = nullptr;
    std::chrono::steady_clock::time_point m_start_time;
};