#pragma once

#include <string>
#include <vector>
#include "Const.h"

class GLObject2D;
class ProgramManager;
class Operation;

class Renderer
{
public:
    Renderer();

    ~Renderer();

    bool initialize(std::string &err);

    void resize(int width, int height);

    void render();

    ProgramManager* programMgr() noexcept;

    void setViewBox(float left, float right, float bttm, float top) noexcept;

    ViewBox* viewBox() const noexcept;

    void addOperation(Operation* operation) noexcept;

    void paintObject(GLObject2D* object) noexcept;

private:
    ProgramManager* m_programMgr = nullptr;
    ViewBox* m_viewBox = nullptr;
    std::vector<Operation*> m_operations;
};