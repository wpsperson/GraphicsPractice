#pragma once

#include <string>
#include <vector>
#include "Const.h"

class GLObject2D;
class ProgramManager;
class FontManager;
class Operation;
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

    ProgramManager* programMgr() noexcept;

    FontManager* fontMgr() noexcept;

    ViewPort* viewPort() const noexcept;

    void addOperation(Operation* operation) noexcept;

    void paintObject(GLObject2D* object) noexcept;

    void paintFont(GLObject2D* object) noexcept;

    static ViewBox toViewBox(const Box& box) noexcept;

    void legacyProjection() noexcept;
public:
    Operation* m_operation = nullptr;

private:
    ProgramManager* m_programMgr = nullptr;
    FontManager* m_fontMgr = nullptr;
    ViewPort* m_viewport = nullptr;
    std::vector<Operation*> m_operations;
};