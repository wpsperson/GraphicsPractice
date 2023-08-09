#include <QtWidgets/QApplication>
#include "OpenGLWidget.h"
#include "Utils.h"

std::string g_argument;

int main(int argc, char *argv[])
{
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
