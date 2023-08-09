#include "DemoOperation.h"

#include <cassert>
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QMouseEvent>

#include "Renderer.h"
#include "ViewPort.h"
#include "ProgramManager.h"
#include "FontManager.h"
#include "glad/glad.h"
#include "OpenGLWidget.h"


void injectAllOperation(Renderer* renderer)
{
    FirstTriangle* ft = new FirstTriangle;
    ft->setName("FirstTriangle");
    renderer->addOperation(ft);
    FontOperation* font = new FontOperation;
    font->setName("FontOperation");
    renderer->addOperation(font);
    MillionPrimitiveOperation* million = new MillionPrimitiveOperation;
    million->setName("MillionPrimitiveOperation");
    renderer->addOperation(million);
    EditVectorFont* edit_font = new EditVectorFont;
    edit_font->setName("EditVectorFont");
    renderer->addOperation(edit_font);
}

FirstTriangle::~FirstTriangle()
{
    if (m_object)
    {
        delete m_object;
    }
}

void FirstTriangle::initialize(Renderer* renderer) noexcept
{
    float len = static_cast<float>(kDefaultDesignSize/2);
    std::vector<Point> pts = { Point(-len, -len), Point(len, -len), Point(len, len) };
    m_object = new GLObject2D(renderer, DrawMode::Fill);
    m_object->setColor(kColorGreen);
    m_object->setOpaque(0.5f);
    m_object->upload(pts.data(), 3);
}

void FirstTriangle::paint(Renderer* renderer) noexcept
{
    renderer->paintObject(m_object);
}

FontOperation::~FontOperation()
{
    if (m_object)
    {
        delete m_object;
    }
}

void FontOperation::initialize(Renderer* renderer) noexcept
{
    FontManager* fontMgr = renderer->fontMgr();
    const AsciiCharInfo *info = fontMgr->asciiChar('A');

    float len = static_cast<float>(kDefaultDesignSize / 2);
    Point pt0(-len, -len);
    Point pt1(len, -len);
    Point pt2(len, len);
    Point pt3(-len, len);
    Point uv0(info->uv_min_x, info->uv_min_y);
    Point uv1(info->uv_max_x, info->uv_min_y);
    Point uv2(info->uv_max_x, info->uv_max_y);
    Point uv3(info->uv_min_x, info->uv_max_y);
    //Point uv0(0.0f, 0.0f);
    //Point uv1(0.8f, 0.0f);
    //Point uv2(0.8f, 0.8f);
    //Point uv3(0.0f, 0.8f);
    std::vector<Point> pts = { pt0, uv0, pt1, uv1, pt2, uv2, pt0, uv0, pt2,uv2, pt3, uv3 };
    m_object = new GLObject2D(renderer, DrawMode::Fill);
    m_object->setColor(kColorGreen);
    m_object->setOpaque(1.0f);
    m_object->upload(pts.data(), 12);
}

void FontOperation::paint(Renderer* renderer) noexcept
{
    renderer->paintFont(m_object);
}

MillionPrimitiveOperation::~MillionPrimitiveOperation()
{
}

void MillionPrimitiveOperation::initialize(Renderer* renderer) noexcept
{
    m_width = kDefaultDesignSize * 0.8;
    m_height = kDefaultDesignSize * 0.8;
}

void MillionPrimitiveOperation::paint(Renderer* renderer) noexcept
{
    float advance = static_cast<float>(m_width / m_repeat_count);

    Point triangle[3];
    triangle[0].x = 0.0, triangle[0].y = 0;
    triangle[1].x = advance, triangle[1].y = 0;
    triangle[2].x = advance / 2, triangle[2].y = advance;
    Point pt0{ 0.0, advance / 2 };
    Point pt1{ advance, advance / 2 };

    float xOffset = 0, yOffset = 0, zOffset = 0;
    float xStep = advance;
    float yStep = advance;

    // batch strategy spend average 100ms, while retail strategy spend average 400ms
    // same-state batch draw vs one by one draw:
    // glBegin draw mode interleave: Triangles/Lines 100ms : 400ms
    // glColor interleave: 150ms : 200 ms
    // enable/disable line stipple 150ms : 1000ms
    // enable line stipple, solid line/phantom line: 100ms/1000ms

    glEnable(GL_LINE_STIPPLE);
    if (m_batch_strategy)
    {
        glColor3f(0.5f, 0.5f, 0.5f);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < m_repeat_count; i++)
        {
            xOffset = i * xStep;
            for (int j = 0; j < m_repeat_count; j++)
            {
                yOffset = j * yStep;
                glVertex2f(triangle[0].x + xOffset, triangle[0].y + yOffset);
                glVertex2f(triangle[1].x + xOffset, triangle[1].y + yOffset);
                glVertex2f(triangle[2].x + xOffset, triangle[2].y + yOffset);
            }
        }
        glEnd();
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < m_repeat_count; i++)
        {
            xOffset = i * xStep;
            for (int j = 0; j < m_repeat_count; j++)
            {
                yOffset = j * yStep;
                glVertex2f(pt0.x + xOffset, pt0.y + yOffset);
                glVertex2f(pt1.x + xOffset, pt1.y + yOffset);
            }
        }
        glEnd();
    }
    else
    {
        glColor3f(0.5f, 0.5f, 0.5f);
        for (int i = 0; i < m_repeat_count; i++)
        {
            xOffset = i * xStep;
            for (int j = 0; j < m_repeat_count; j++)
            {
                yOffset = j * yStep;
                glBegin(GL_TRIANGLES);
                glVertex2f(triangle[0].x + xOffset, triangle[0].y + yOffset);
                glVertex2f(triangle[1].x + xOffset, triangle[1].y + yOffset);
                glVertex2f(triangle[2].x + xOffset, triangle[2].y + yOffset);
                glEnd();
                glBegin(GL_LINES);
                glVertex2f(pt0.x + xOffset, pt0.y + yOffset);
                glVertex2f(pt1.x + xOffset, pt1.y + yOffset);
                glEnd();
            }
        }
    }
}

EditVectorFont::~EditVectorFont()
{
    delete m_old_points;
    delete m_old_lines;
    delete m_new_points;
    delete m_new_lines;
    delete m_snap;
}

void EditVectorFont::initialize(Renderer* renderer) noexcept
{
    m_old_points = new GLObject2D(renderer, DrawMode::Points);
    m_old_points->setColor(kColorBlue);
    m_old_lines = new GLObject2D(renderer, DrawMode::Lines);
    m_old_lines->setColor(kColorBlue);
    m_new_points = new GLObject2D(renderer, DrawMode::Points);
    m_new_points->setColor(kColorCyan);
    m_new_lines = new GLObject2D(renderer, DrawMode::Lines);
    m_new_lines->setColor(kColorCyan);
    m_grid = new GLObject2D(renderer, DrawMode::Lines);
    m_grid->setColor(kColorGray);
    m_grid->setOpaque(0.5f);
    m_grid2 = new GLObject2D(renderer, DrawMode::Lines);
    m_grid2->setColor(kColorGray);
    m_grid2->setOpaque(0.8f);
    m_snap = new GLObject2D(renderer, DrawMode::Points);
    m_snap->setColor(kColorRed);

    parseFontFile("old_font.txt", kOldFontDim, m_old_library);
    parseFontFile("new_font.txt", kNewFontDim, m_new_library);
    m_vp = renderer->viewPort();
    overallMode();
}

void EditVectorFont::paint(Renderer* renderer) noexcept
{
    if (m_edit_mode)
    {
        renderer->paintObject(m_grid);
        renderer->paintObject(m_grid2);
    }
    renderer->paintObject(m_old_points);
    renderer->paintObject(m_old_lines);
    renderer->paintObject(m_new_points);
    renderer->paintObject(m_new_lines);
    if (m_edit_mode && m_snap->count())
    {
        renderer->paintObject(m_snap);
    }
}

void EditVectorFont::setWidget(OpenGLWidget* widget)
{
    m_widget = widget;
}

void EditVectorFont::processMouseClick(QMouseEvent* eve)
{
    int x = eve->pos().x();
    int y = eve->pos().y();
    if (eve->button() == Qt::LeftButton)
    {
        if (m_edit_mode)
        {
            m_drag = true;
            selectPoint(x, y);
            m_widget->update();
        }
    }
    else if (eve->button() == Qt::MiddleButton)
    {
        selectEditChar(x, y);
    }
    else if (eve->button() == Qt::RightButton)
    {

    }
}

void EditVectorFont::processMouseRelease(QMouseEvent* eve)
{
    if (eve->button() == Qt::LeftButton)
    {
        m_drag = false;
    }
    else if (eve->button() == Qt::MiddleButton)
    {

    }
    else if (eve->button() == Qt::RightButton)
    {

    }
}

void EditVectorFont::processDoubleMouseClick(QMouseEvent* eve)
{

}

void EditVectorFont::processMouseMove(QMouseEvent* eve)
{
    if (!m_edit_mode || !m_drag || !m_stroke)
    {
        return;
    }

    int scrx = eve->pos().x();
    int scry = eve->pos().y();
    moveCurrentPoint(scrx, scry);
    m_widget->update();
}

void EditVectorFont::processKeyPress(QKeyEvent* event)
{
    int key = event->key();
    if (Qt::Key_F1 == key)
    {
        overallMode();
        m_widget->update();
    }
    if (Qt::Key_F6 == key)
    {

        m_widget->update();
    }

    if ((event->modifiers() & Qt::ControlModifier))
    {
        if (Qt::Key_S == key)
        {
            saveNewVectorFont();
        }
        if (Qt::Key_T == key)
        {
            std::string backup_file = "new_format_font_" + timeToStr("%H-%M") + ".txt";
            writeFontFileInNewFormat(backup_file.c_str(), m_new_library);
        }
        if (Qt::Key_C == key)
        {
            checkFontToNewFormat();
        }
    }
}

void EditVectorFont::updatePointObject(const VFontLibrary& lib, GLObject2D* points, unsigned char only_char) noexcept
{
    std::vector<Point> buffer;
    buffer.reserve(4096);

    for (int i = 33; i <= 126; i++)
    {
        if (only_char > 0 && only_char != i)
        {
            continue;
        }
        const VFont& font = lib[i];
        int left = (i % 10) * kViewSize;
        int bttm = (i / 10) * kViewSize;
        for (const VStroke& sk : font.strokes)
        {
            for (const VPoint& pt : sk.points)
            {
                float x = static_cast<float>(left + pt.normx() * kViewSize);
                float y = static_cast<float>(bttm + pt.normy() * kViewSize);
                buffer.emplace_back(x, y);
            }
        }
    }
    int pt_count = static_cast<int>(buffer.size());
    Point* data = buffer.data();
    points->upload(data, pt_count);
}

void EditVectorFont::updateLinesObject(const VFontLibrary& lib, GLObject2D* lines, unsigned char only_char) noexcept
{
    std::vector<Point> buffer;
    buffer.reserve(4096);

    for (int i = 33; i <= 126; i++)
    {
        if (only_char > 0 && only_char != i)
        {
            continue;
        }
        const VFont& font = lib[i];
        int left = (i % 10) * kViewSize;
        int bttm = (i / 10) * kViewSize;
        for (const VStroke& sk : font.strokes)
        {
            int count = static_cast<int>(sk.points.size());
            for (int i = 0; i < count;i++)
            {
                const VPoint& pt = sk.points[i];

                float x = static_cast<float>(left + pt.normx() * kViewSize);
                float y = static_cast<float>(bttm + pt.normy() * kViewSize);
                buffer.emplace_back(x, y);
                if (i != 0 && i != count - 1)
                {
                    buffer.emplace_back(x, y);
                }
            }
        }
    }
    int pt_count = static_cast<int>(buffer.size());
    Point* data = buffer.data();
    lines->upload(data, pt_count);
}

void EditVectorFont::updateGridObject(int left, int bttm, int width, int height, int repeat, GLObject2D* grid) noexcept
{
    std::vector<Point> buffer;
    int deltax = width / repeat;
    int deltay = height / repeat;
    float y = 0.0f;
    float x0 = static_cast<float>(left);
    float x1 = static_cast<float>(left + width);
    for (int row = 0; row <= repeat; row++)
    {
        y = static_cast<float>(bttm + row * deltay);
        buffer.emplace_back(x0, y);
        buffer.emplace_back(x1, y);
    }

    float x = 0.0f;
    float y0 = static_cast<float>(bttm);
    float y1 = static_cast<float>(bttm + height);
    for (int col = 0; col <= repeat; col++)
    {
        x = static_cast<float>(left + col * deltax);
        buffer.emplace_back(x, y0);
        buffer.emplace_back(x, y1);
    }

    Point* data = buffer.data();
    int pt_count = static_cast<int>(buffer.size());
    grid->upload(data, pt_count);
}

void EditVectorFont::updateSnapObject(int x, int y, GLObject2D* m_snap) noexcept
{
    Point pt{ float(x), float(y) };
    m_snap->upload(&pt, 1);
}

void EditVectorFont::saveNewVectorFont() noexcept
{
    writeFontFile("new_font.txt", m_new_library);
    std::string backup_file = "new_size_font_" + timeToStr("%H-%M") + ".txt";
    writeFontFile(backup_file.c_str(), m_new_library);
}

void EditVectorFont::checkFontToNewFormat() noexcept
{
    for (VFont& ft : m_new_library)
    {
        for (VStroke& sk : ft.strokes)
        {
            int cmd_count = sk.cmds.size();
            int pt_count = sk.points.size();
            assert(cmd_count==pt_count);
            VPoint last = sk.points.front();
            sk.cmds[0] = 1;
            for (int i = 1; i < pt_count; i++)
            {
                VPoint cur = sk.points.at(i);
                if (cur.y == last.y)
                {
                    sk.cmds[i] = 2;
                }else if (cur.x == last.x)
                {
                    sk.cmds[i] = 3;
                }
                else
                {
                    sk.cmds[i] = 4;
                }
                last = cur;
            }
        }
    }
}

void EditVectorFont::overallMode()
{
    m_edit_mode = false;
    updatePointObject(m_old_library, m_old_points, 0);
    updateLinesObject(m_old_library, m_old_lines, 0);
    updatePointObject(m_new_library, m_new_points, 0);
    updateLinesObject(m_new_library, m_new_lines, 0);

    glint64 ext = kViewSize;
    glint64 left = 0 - ext;
    glint64 bttm = 3 * kViewSize - ext;
    glint64 width = 10 * kViewSize + ext * 2;
    m_vp->setDesign(left, bttm, width, width);
    m_vp->updateViewToDesign();
}

void EditVectorFont::editFontMode()
{
    if (m_cur_char == 0)
    {
        return;
    }
    m_edit_mode = true;

    updatePointObject(m_old_library, m_old_points, m_cur_char);
    updateLinesObject(m_old_library, m_old_lines, m_cur_char);
    updatePointObject(m_new_library, m_new_points, m_cur_char);
    updateLinesObject(m_new_library, m_new_lines, m_cur_char);
    int left = (m_cur_char % 10) * kViewSize;
    int bttm = (m_cur_char / 10) * kViewSize;
    int width = kViewSize;
    int height = kViewSize;
    updateGridObject(left, bttm, width, height, 100, m_grid);
    updateGridObject(left, bttm, width, height, 10, m_grid2);

    int leftext = (m_cur_char % 10) * kViewSize - kViewSize / 2;
    int bttmext = (m_cur_char / 10) * kViewSize - kViewSize / 2;
    int widthext = kViewSize * 2;
    int heightext = kViewSize * 2;
    m_vp->setView(leftext, bttmext, widthext, heightext);
    m_widget->update();
}

void EditVectorFont::selectEditChar(int screen_x, int screen_y)
{
    llPoint pt = m_vp->screenToDB(screen_x, screen_y);
    int x = static_cast<int>(pt.x);
    int y = static_cast<int>(pt.y);
    int col = (x / kViewSize);
    int row = (y / kViewSize);
    m_cur_char = static_cast<unsigned char>(row * 10 + col);
    m_font = &(m_new_library[m_cur_char]);
    editFontMode();
}

void EditVectorFont::resetEditChar()
{
    m_cur_char = 0;
    m_font = nullptr;
    m_stroke = nullptr;
    m_pt = 0;
}

void EditVectorFont::selectPoint(int screen_x, int screen_y)
{
    llPoint pt = m_vp->screenToDB(screen_x, screen_y);
    int x = static_cast<int>(pt.x);
    int y = static_cast<int>(pt.y);
    int col = m_cur_char % 10;
    int row = m_cur_char / 10;
    int localx = x - col * kViewSize;
    int localy = y - row * kViewSize;
    constexpr int eps = 5;
    m_stroke = nullptr;
    m_pt = -1;
    for (VStroke &sk : m_font->strokes)
    {
        for (int i=0; i< sk.points.size(); i++)
        {
            VPoint& pt = sk.points[i];
            int ptx = pt.normx() * kViewSize;
            int pty = pt.normy() * kViewSize;

            if (std::abs(ptx - localx) <= eps && std::abs(pty - localy) <= eps)
            {
                m_stroke = &sk;
                m_pt = i;
                break;
            }
        }
        if (m_stroke)
        {
            break;
        }
    }
    // update the highlight
    if (m_stroke)
    {
        Point pt;
        VPoint vpt = m_stroke->points[m_pt];
        int ptx = col * kViewSize + vpt.normx() * kViewSize;
        int pty = row * kViewSize + vpt.normy() * kViewSize;
        updateSnapObject(ptx, pty, m_snap);
    }
}

void EditVectorFont::moveCurrentPoint(int screen_x, int screen_y)
{
    llPoint pt = m_vp->screenToDB(screen_x, screen_y);
    double normx, normy;
    worldToNormal(pt, normx, normy);
    m_stroke->points[m_pt].setNorm(normx, normy);

    updatePointObject(m_new_library, m_new_points, m_cur_char);
    updateLinesObject(m_new_library, m_new_lines, m_cur_char);
    updateSnapObject(pt.x, pt.y, m_snap);

}

void EditVectorFont::worldToNormal(const llPoint& pt, double& normx, double& normy)
{
    int row = m_cur_char / 10;
    int col = m_cur_char % 10;
    int localx = pt.x - col * kViewSize;
    int localy = pt.y - row * kViewSize;
    normx = static_cast<double>(localx) / kViewSize;
    normy = static_cast<double>(localy) / kViewSize;
}

void EditVectorFont::localToWorld(int localx, int localy, int& worldx, int& worldy)
{
    int row = m_cur_char / 10;
    int col = m_cur_char % 10;
    worldx = localx + col * kViewSize;
    worldy = localy + row * kViewSize;
}

