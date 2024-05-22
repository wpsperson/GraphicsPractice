#include <QtWidgets/QApplication>
#include "OpenGLWidget.h"
#include "DummyWidget.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //OpenGLWidget* dialog = new OpenGLWidget;
    //dialog->show();

    //DummyWidget dummy;
    //dummy.show();

    MainWindow mainwindow;
    mainwindow.show();

    return a.exec();
}
