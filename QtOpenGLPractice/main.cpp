#include <QtWidgets/QApplication>
#include <QtCore/QString>
#include "OpenGLWidget.h"
#include "Utils.h"

std::string g_argument;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString argument;
    if (argc >= 2)
    {
        argument = QString(argv[1]);
    }
    OpenGLWidget* dialog = new OpenGLWidget;
    dialog->setObjectName(argument);
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->show();
    return a.exec();
}
