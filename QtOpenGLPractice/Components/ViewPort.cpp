#include "Components/ViewPort.h"
#include <corecrt_math.h>
#include <cmath>

void ViewPort::setDesign(glint64 left, glint64 bttm, glint64 width, glint64 height)
{
    m_design.setRelative(left, bttm, width, height);
}

void ViewPort::setView(glint64 left, glint64 bttm, glint64 width, glint64 height)
{
    m_view.setRelative(left, bttm, width, height);
    // update the 
    double ratio = static_cast<double>(width) / m_screen_width;
    m_pixel_length = ratio;
}

void ViewPort::setScreen(glint64 screen_width, glint64 screen_height)
{
    m_screen_width = screen_width;
    m_screen_height = screen_height;
}

const Box& ViewPort::getDesign() const
{
    return m_design;
}

const Box& ViewPort::getView() const
{
    return m_view;
}

llPoint ViewPort::getViewPort() const
{
    return llPoint{ m_screen_width, m_screen_height };
}

void ViewPort::getViewPort(glint64& screen_width, glint64& screen_height) const
{
    screen_width = m_screen_width;
    screen_height = m_screen_height;
}

void ViewPort::zoomView(glint64 x, glint64 y, double zoom_factor)
{
    // here we could add some constrain for new pixel length
    double new_pixel_len = m_pixel_length * zoom_factor;
    if (new_pixel_len < 0.1 && zoom_factor < 1.0)
    {
        return;
    }

    glint64 left_dist = x - m_view.left();
    glint64 bttm_dist = y - m_view.bottom();
    glint64 new_left_dist = std::llround(left_dist * zoom_factor);
    glint64 new_bttm_dist = std::llround(bttm_dist * zoom_factor);
    glint64 new_left = x - new_left_dist;
    glint64 new_bttm = y - new_bttm_dist;
    glint64 new_width = std::llround(m_screen_width * new_pixel_len);
    glint64 new_height = std::llround(m_screen_height * new_pixel_len);
    if (new_width >= m_design.width() && new_height >= m_design.height())
    {
        updateViewToDesign();
        return;
    }
    constrainToDesign(new_left, new_bttm, new_width, new_height);
    setView(new_left, new_bttm, new_width, new_height);
}

void ViewPort::moveView(glint64 xoffset, glint64 yoffset)
{
    glint64 left = m_view.left();
    glint64 bttm = m_view.bottom();
    glint64 width = m_view.width();
    glint64 height = m_view.height();
    glint64 new_left = left + xoffset;
    glint64 new_bttm = bttm + yoffset;
    constrainToDesign(new_left, new_bttm, width, height);
    setView(new_left, new_bttm, width, height);
}

void ViewPort::constrainToDesign(glint64& left, glint64& bttm, glint64 width, glint64 height)
{
    if (left < m_design.left() || width >= m_design.width())
    {
        left = m_design.left();
    }
    else if (left + width > m_design.right())
    {
        left = m_design.right() - width;
    }

    if (bttm + height > m_design.top() || height >= m_design.height())
    {
        bttm = m_design.top() - height;
    }
    else if (bttm < m_design.bottom())
    {
        bttm = m_design.bottom();
    }
}

// we adjust the view box so that:
// 1 the view has same aspect with screen
// 2 the view contain the whole design
// 3 the view left top corner is exact same with desgin
void ViewPort::updateViewToDesign()
{
    double ratiox = static_cast<double>(m_design.width()) / m_screen_width;
    double ratioy = static_cast<double>(m_design.height()) / m_screen_height;
    double new_pixel_len = std::max(ratiox, ratioy);
    glint64 new_width = std::llround(m_screen_width * new_pixel_len);
    glint64 new_height = std::llround(m_screen_height * new_pixel_len);
    glint64 new_left = m_design.left();
    glint64 new_bttm = m_design.top() - new_height;
    setView(new_left, new_bttm, new_width, new_height);
}

double ViewPort::pixelLength() const
{
    return m_pixel_length;
}

llPoint ViewPort::screenToDB(int x, int y) const
{
    glint64 pt_x = m_view.left() + std::llround(x * m_pixel_length);
    glint64 pt_y = m_view.top() - std::llround(y * m_pixel_length);
    return llPoint(pt_x, pt_y);
}

bool ViewPort::dbToScreen(const llPoint& pt, int& screen_x, int& screen_y) const
{
    if (!m_view.containPoint(pt))
    {
        return false;
    }
    glint64 left_dist = pt.x - m_view.left();
    glint64 top_dist = m_view.top() - pt.y;
    screen_x = static_cast<int>(left_dist / m_pixel_length);
    screen_y = static_cast<int>(top_dist / m_pixel_length);
    return true;
}


