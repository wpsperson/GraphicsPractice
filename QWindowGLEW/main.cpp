#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 设置OpenGL格式
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);
    format.setSamples(4); // 4x MSAA
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow window;
    window.show();

    return app.exec();
}

//#include <QGuiApplication>
//#include <QWindow>
//
//int main(int argc, char** argv)
//{
//    QGuiApplication app(argc, argv);
//
//    QWindow window;
//    window.setTitle("A Simple QWindow");
//    window.resize(800, 600);
//    window.show();
//
//    return app.exec();
//}