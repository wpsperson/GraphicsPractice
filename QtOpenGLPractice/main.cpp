#include <QtWidgets/QApplication>
#include "OpenGLWidget.h"
//#include "GlyphOutlineGenerator.h"

std::string g_argument;

int main(int argc, char *argv[])
{
    //GlyphOutlineGenerator gen;
    //std::string font_file_name = std::getenv("SystemRoot") + std::string("/Fonts/simfang.ttf");
    //std::string err_msg;
    //if (!gen.initFontLibrary(err_msg, font_file_name))
    //{
    //    return 0;
    //}
    //wchar_t ch = L'人';
    //wchar_t ch2 = L'口';
    //GlyphOutlines outln;
    //bool suc = gen.genGlyphOutlines(ch, err_msg, outln);
    //suc = gen.genGlyphOutlines(ch2, err_msg, outln);
    //return false;

    QApplication a(argc, argv);
    if (argc >= 2)
    {
        g_argument = std::string(argv[1]);
    }
    OpenGLWidget* dialog = new OpenGLWidget;
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
    return a.exec();
}
