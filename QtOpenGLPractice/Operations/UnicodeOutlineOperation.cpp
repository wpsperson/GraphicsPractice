#include "Operations/UnicodeOutlineOperation.h"

#include <iostream>
#include <filesystem>
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QMouseEvent>

#include "Components/Renderer.h"
#include "Components/ViewPort.h"
#include "Data/AllChineseChar.h"
#include "OpenGLWidget.h"

#pragma warning(disable: 4996)
namespace fs = std::filesystem;

UnicodeOutlineOperation::~UnicodeOutlineOperation()
{
    if (m_object)
    {
        delete m_object;
    }
}

void UnicodeOutlineOperation::initialize(Renderer* renderer) noexcept
{
    std::vector<std::string> all_fonts = allSystemFontFiles();
    for (const std::string& font : all_fonts)
    {
        if (supportChineseOutline(font))
        {
            m_font_list.emplace_back(font);
        }
    }
    // m_font_list;
    std::string font_file_name = m_font_list[m_font_idx];
    std::string err_msg;
    if (!m_generator.initFontLibrary(err_msg, font_file_name))
    {
        return;
    }
    // wchar_t ch = L'人';
    size_t size = kAllChineseChars.size();
    for (size_t idx = 0; idx < size; idx++)
    {
        wchar_t ch = kAllChineseChars[idx];
        if (!m_generator.genGlyphOutlines(ch, err_msg, m_outln))
        {
            std::cout << err_msg << std::endl;
            return;
        }
    }

    m_object = new GLObject2D(renderer, DrawMode::Lines);
    m_object->setColor(kColorGreen);
    m_object->setOpaque(0.5f);
    m_init_success = true;
}

void UnicodeOutlineOperation::paint(Renderer* renderer) noexcept
{
    if (!m_init_success)
    {
        return;
    }
    ViewPort* vp = renderer->viewPort();
    double pixel_size = vp->pixelLength();
    Box box = vp->getDesign();
    glint64 width = box.width() / 2;
    glint64 height = box.height() / 2;
    glint64 left = box.left() + width /2;
    glint64 bttm = box.bottom() + height/2;

    int dim = 60;
    glint64 spacex = width / dim;
    glint64 spacey = height / dim;
    glint64 char_w = spacex;
    glint64 char_h = spacey;

    std::string err_msg;
    size_t char_count = kAllChineseChars.size();
    for (size_t idx = 0; idx < char_count; idx++)
    {
        wchar_t ch = kAllChineseChars[idx];
        if (!m_generator.genGlyphOutlines(ch, err_msg, m_outln))
        {
            std::cout << err_msg << std::endl;
            return;
        }
        glint64 offx = left + (idx % dim) * spacex;
        glint64 offy = bttm + (idx / dim) * spacey;
        GlyphOutlineGenerator::discretizeOutline(m_outln, offx, offy, char_w, char_h, 10, m_mesh);

        int size = static_cast<int>(m_mesh.size());
        m_object->upload(m_mesh.data(), size);
        renderer->paintObject(m_object);
    }

}

void UnicodeOutlineOperation::processKeyPress(QKeyEvent* event)
{
    int key = event->key();
    if (Qt::Key_PageDown == key)
    {
        m_font_idx++;
        if (m_font_idx >= m_font_list.size())
        {
            m_font_idx = 0;
        }
        loadFont(m_font_idx);
        m_widget->update();
    }
    else if (Qt::Key_PageUp == key)
    {
        m_font_idx--;
        if (m_font_idx < 0)
        {
            m_font_idx = int(m_font_list.size() - 1);
        }
        loadFont(m_font_idx);
        m_widget->update();
    }
}

void UnicodeOutlineOperation::loadFont(int index) noexcept
{
    std::string font_file_name = m_font_list[m_font_idx];
    std::cout << "load font: " << font_file_name << std::endl;
    std::string err_msg;
    m_generator.clearOutlines();
    if (!m_generator.initFontLibrary(err_msg, font_file_name))
    {
        return;
    }
    // wchar_t ch = L'人';
    size_t size = kAllChineseChars.size();
    for (size_t idx = 0; idx < size; idx++)
    {
        wchar_t ch = kAllChineseChars[idx];
        if (!m_generator.genGlyphOutlines(ch, err_msg, m_outln))
        {
            std::cout << err_msg << std::endl;
            return;
        }
    }
}

std::vector<std::string> UnicodeOutlineOperation::allSystemFontFiles() noexcept
{
    struct FontFile
    {
        std::string file_path;
        std::string stem;
        bool derived = false;
    };
    std::vector<FontFile> fonts_list;
    std::string folder = std::getenv("SystemRoot") + std::string("/Fonts/");
    for (const fs::directory_entry& entry : fs::directory_iterator(folder))
    {
        if (entry.is_directory())
        {
            continue;
        }
        std::string ext_name = entry.path().extension().string();
        if (ext_name != ".ttf" && ext_name != ".ttc" && ext_name != ".TTF" && ext_name != ".TTC") // truetype font/ truetype collection
        {
            continue;
        }
        FontFile ff;
        ff.stem = entry.path().stem().string();
        ff.file_path = entry.path().string();
        fonts_list.emplace_back(ff);
    }
    size_t count = fonts_list.size();
    for (size_t idx = 0; idx < count; idx++)
    {
        const std::string& str = fonts_list[idx].stem;
        for (size_t i = idx + 1; i < count;i++)
        {
            const std::string& another = fonts_list[i].stem;
            if (another.find(str) != std::string::npos)
            {
                fonts_list[i].derived = true;
            }
        }
    }

    std::vector<std::string> results;
    for (size_t idx = 0; idx < count; idx++)
    {
        if (!fonts_list[idx].derived)
        {
            results.emplace_back(fonts_list[idx].file_path);
        }
    }
    return results;
}

bool UnicodeOutlineOperation::supportChineseOutline(const std::string& font_file) noexcept
{
    std::string err_msg;
    m_generator.clearOutlines();
    if (!m_generator.initFontLibrary(err_msg, font_file))
    {
        std::cout << err_msg << std::endl;
        return false;
    }

    wchar_t ch_zhong = L'中';
    wchar_t ch_guo = L'国';
    GlyphOutlines outln_zhong;
    GlyphOutlines outln_guo;
    if (!m_generator.genGlyphOutlines(ch_zhong, err_msg, outln_zhong))
    {
        std::cout << err_msg << std::endl;
        return false;
    }
    if (!m_generator.genGlyphOutlines(ch_guo, err_msg, outln_guo))
    {
        std::cout << err_msg << std::endl;
        return false;
    }

    if (exactSameOutln(outln_zhong, outln_guo))
    {
        return false;
    }
    return true;
}

bool UnicodeOutlineOperation::exactSameOutln(const GlyphOutlines& outln1, const GlyphOutlines& outln2) noexcept
{
    if (outln1.loops.size() != outln2.loops.size())
    {
        return false;
    }
    size_t loop_count = outln1.loops.size();
    for (size_t idx = 0; idx <loop_count; idx++)
    {
        const GlyphLoop& loop1 = outln1.loops[idx];
        const GlyphLoop& loop2 = outln2.loops[idx];
        if (loop1.coords.size() != loop2.coords.size() || loop1.cmds.size() != loop2.cmds.size())
        {
            return false;
        }
    }
    return true;
}

