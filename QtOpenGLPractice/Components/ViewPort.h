#pragma once

#include "Core/DefineStruct.h"
#include "Core/Box.h"

class ViewPort
{
public:
    void setDesign(glint64 left, glint64 bttm, glint64 width, glint64 height);

    void setView(glint64 left, glint64 bttm, glint64 width, glint64 height);

    void setScreen(glint64 screen_width, glint64 screen_height);

    const Box& getDesign() const;

    const Box& getView() const;

    llPoint getViewPort() const;

    void getViewPort(glint64 &screen_width, glint64 &screen_height) const;

    void zoomView(glint64 centx, glint64 centy, double zoom_factor);

    void moveView(glint64 xoffset, glint64 yoffset);

    void constrainToDesign(glint64 &left, glint64 &bttm, glint64 width, glint64 height);

    void updateViewToDesign();

    double pixelLength() const;

    llPoint screenToDB(int x, int y) const;

    bool dbToScreen(const llPoint& pt, int& screen_x, int& screen_y) const;

private:
    Box m_design;
    Box m_view;
    glint64 m_screen_width = 0;
    glint64 m_screen_height = 0;
    double m_pixel_length;
};


