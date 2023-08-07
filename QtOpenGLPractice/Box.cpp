#include "Box.h"

Box::Box(glint64 left, glint64 bttm, glint64 width, glint64 height)
    :xmin(left), ymin(bttm), xmax(left+width), ymax(bttm+height)
{
}

void Box::setCentWidthHeight(glint64 centx, glint64 centy, glint64 width, glint64 height)
{
    xmin = centx - width / 2;
    ymin = centy - height / 2;
    xmax = centx + width / 2;
    ymax = centy + height / 2;
}

void Box::setRelative(glint64 left, glint64 bttm, glint64 width, glint64 height)
{
    xmin = left;
    ymin = bttm;
    xmax = left + width;
    ymax = bttm + height;
}

void Box::setAbsolute(glint64 left, glint64 bttm, glint64 right, glint64 top)
{
    xmin = left;
    ymin = bttm;
    xmax = right;
    ymax = top;
}

glint64 Box::left() const
{
    return xmin;
}

glint64 Box::right() const
{
    return xmax;
}

glint64 Box::bottom() const
{
    return ymin;
}

glint64 Box::top() const
{
    return ymax;
}

glint64 Box::width() const
{
    return xmax - xmin;
}

glint64 Box::height() const
{
    return ymax - ymin;
}

bool Box::containPoint(const llPoint& pt) const
{
    return containPoint(pt.x, pt.y);
}
bool Box::containPoint(glint64 ptx, glint64 pty) const
{
    if (ptx > xmax || ptx < xmin)
        return false;
    if (pty > ymax || pty < ymin)
        return false;
    return true;
}
