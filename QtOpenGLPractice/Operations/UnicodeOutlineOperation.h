#pragma once

#include "Operations/Operation.h"

#include <vector>

#include "GLObject2D.h"
#include "Utils.h"
#include "GlyphOutlineGenerator.h"

class UnicodeOutlineOperation : public Operation
{
public:
    ~UnicodeOutlineOperation();

    void initialize(Renderer* renderer) noexcept override;

    void paint(Renderer* renderer) noexcept override;

    void processKeyPress(QKeyEvent* event) override;

    void loadFont(int index) noexcept;

    static std::vector<std::string> allSystemFontFiles() noexcept;

    bool supportChineseOutline(const std::string& font_file) noexcept;

    bool exactSameOutln(const GlyphOutlines& outln1, const GlyphOutlines& outln2) noexcept;

private:
    GLObject2D* m_object = nullptr;
    GlyphOutlines m_outln;
    GlyphOutlineGenerator m_generator;
    std::vector<Point> m_mesh;
    bool m_init_success = false;
    std::vector<std::string> m_font_list;
    int m_font_idx = 0;
};