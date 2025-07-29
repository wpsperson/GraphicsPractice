#pragma once

#include "Operations/Operation.h"
#include "Components/GLObject2D.h"


class FBOOperation : public Operation
{
public:
    FBOOperation();

    ~FBOOperation();

    void initialize(Renderer* renderer) noexcept override;

    void paint(Renderer* renderer) noexcept override;

    void resizeOperation(int width, int height) noexcept override;

    void processMouseClick(QMouseEvent* eve) override;

    void processMouseRelease(QMouseEvent* eve) override;

    void processMouseMove(QMouseEvent* eve) override;

    void processMouseWheel(QWheelEvent* eve) override;

private:
    bool initFBO();

    void drawStaticScene();

    void applyTexture();

    void drawDynamicScene();

private:
    bool m_support_fbo = false;
    Renderer* m_renderer = nullptr;
    GLObject2D* m_static = nullptr;
    GLObject2D* m_sten_obj = nullptr;
    GLObject2D* m_sten_void = nullptr;
    GLObject2D* m_tex_obj = nullptr;
    GLObject2D* m_dynamic = nullptr;
    unsigned int m_fbo = 0;
    unsigned int m_texture = 0;
    unsigned int m_stencil = 0;
    bool m_texture_done = false;
    int m_device_width = 0;
    int m_device_height = 0;
    // mouse trace
    bool m_drag = false;
    llPoint m_mouse_pt;
};