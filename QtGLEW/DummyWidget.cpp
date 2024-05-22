#include "DummyWidget.h"


#include <QtWidgets/QDockWidget>
#include <QtWidgets/QListWidget>

#include "OpenGLWidget.h"

DummyWidget::DummyWidget(QWidget* parent /*= nullptr*/)
    :QMainWindow(parent)
{
    canvas = new OpenGLWidget(this);
    canvas->setPopup(true);
    setCentralWidget(canvas);

    QDockWidget* dock = new QDockWidget(tr("Customers"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QListWidget* customerList = new QListWidget(dock);
    customerList->addItems(QStringList()
        << "Dummy"
        << "Dummy"
        << "Dummy"
        << "Dummy"
        << "Dummy"
        << "Dummy"
        << "Dummy");
    dock->setWidget(customerList);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    resize(800, 600);
}

DummyWidget::~DummyWidget()
{

}

