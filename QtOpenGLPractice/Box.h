#pragma once
#include "Const.h"

struct Box
{
    Box() = default;

    Box(glint64 left, glint64 bttm, glint64 width, glint64 height);

    void setCentWidthHeight(glint64 centx, glint64 centy, glint64 width, glint64 height);

    void setRelative(glint64 left, glint64 bttm, glint64 width, glint64 height);

    void setAbsolute(glint64 left, glint64 bttm, glint64 right, glint64 top);

    glint64 left() const;

    glint64 right() const;

    glint64 bottom() const;

    glint64 top() const;

    glint64 width() const;

    glint64 height() const;

    bool containPoint(const llPoint& pt) const;
    bool containPoint(glint64 ptx, glint64 pty) const;

public:
    glint64 xmin = 0;
    glint64 ymin = 0;
    glint64 xmax = 0;
    glint64 ymax = 0;
};