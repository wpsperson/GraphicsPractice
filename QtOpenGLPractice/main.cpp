#include <QtWidgets/QApplication>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include "OpenGLWidget.h"
#include "Util/ArgumentUtil.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    ArgumentUtil::initArguments(argc, argv);
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
