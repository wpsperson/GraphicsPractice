#include "MainWindow.h"

#include "QAction"
#include "QMainWindow"
#include "QLabel"
#include "QMenuBar"
#include "QToolBar"
#include "QStatusBar"
#include "QApplication"
#include "QMessageBox"
#include "QFileDialog"
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QListWidget>

#include "OpenGLWidget.h"
#include "DummyWidget.h"

MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    :QMainWindow(parent)
{
    canvas = new OpenGLWidget(this);
    setCentralWidget(canvas);

    createActions();
    createMenus();

    QDockWidget* dock = new QDockWidget(tr("Customers"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QListWidget *customerList = new QListWidget(dock);
    customerList->addItems(QStringList()
        << "Hello"
        << "World"
        << "Good");
    dock->setWidget(customerList);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    resize(800, 600);
}

MainWindow::~MainWindow()
{

}


void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
}


void MainWindow::newFile()
{
    DummyWidget* widget = new DummyWidget;
    widget->show();
}
