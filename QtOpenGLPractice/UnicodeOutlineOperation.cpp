#include "UnicodeOutlineOperation.h"

#include <iostream>

#include "Renderer.h"
#include "ViewPort.h"
#include "AllChineseChar.h"

#pragma warning(disable: 4996)

UnicodeOutlineOperation::~UnicodeOutlineOperation()
{
    if (m_object)
    {
        delete m_object;
    }
}

void UnicodeOutlineOperation::initialize(Renderer* renderer) noexcept
{
    std::string font_file_name = std::getenv("SystemRoot") + std::string("/Fonts/simfang.ttf");
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

