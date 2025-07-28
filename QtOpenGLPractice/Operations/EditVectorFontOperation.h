#pragma once
#include "Operations/Operation.h"

#include <vector>

#include "GLObject2D.h"
#include "Util/Utils.h"

class ViewPort;
class QMouseEvent;
class QKeyEvent;

class EditVectorFont : public Operation
{
public:
    ~EditVectorFont();

    virtual void initialize(Renderer* renderer) noexcept;

    virtual void paint(Renderer* renderer) noexcept;

    void processMouseClick(QMouseEvent* eve);

    void processMouseRelease(QMouseEvent* eve);

    void processDoubleMouseClick(QMouseEvent* eve);

    void processMouseMove(QMouseEvent* eve);

    void processKeyPress(QKeyEvent* event);

private:
    void updatePointObject(const VFontLibrary& lib, GLObject2D* points, unsigned char only_char) noexcept;

    void updateLinesObject(const VFontLibrary& lib, GLObject2D* lines, unsigned char only_char) noexcept;

    void updateGridObject(int left, int bttm, int width, int height, int repeat, GLObject2D* grid) noexcept;

    void updateSnapObject(int x, int y, GLObject2D* m_snap) noexcept;

    void saveNewVectorFont() noexcept;

    void checkFontToNewFormat() noexcept;

    void overallMode();

    void editFontMode();

    void selectEditChar(int screen_x, int screen_y);

    void resetEditChar();

    void selectPoint(int screen_x, int screen_y);

    void moveCurrentPoint(int screen_x, int screen_y);

    void worldToNormal(const llPoint& pt, double& normx, double& normy);

    void localToWorld(int localx, int localy, int& worldx, int& worldy);

private:
    VFontLibrary m_old_library;
    VFontLibrary m_new_library;
    GLObject2D* m_old_points = nullptr;
    GLObject2D* m_old_lines = nullptr;
    GLObject2D* m_new_points = nullptr;
    GLObject2D* m_new_lines = nullptr;
    GLObject2D* m_grid = nullptr;
    GLObject2D* m_grid2 = nullptr;
    GLObject2D* m_snap = nullptr;
    unsigned char m_cur_char = 0;
    VFont* m_font = nullptr;
    VStroke* m_stroke = nullptr;
    int m_pt = -1; // pt index in stroke

    ViewPort* m_vp;
    bool m_edit_mode = false;
    bool m_drag = false;
};