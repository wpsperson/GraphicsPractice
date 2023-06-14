#include <QtWidgets/QApplication>
#include "CanvasWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CanvasWidget* dialog = new CanvasWidget;
    dialog->show();
    return a.exec();
}
