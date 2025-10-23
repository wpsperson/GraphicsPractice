#include "mainwindow.h"
#include "openglwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_container(nullptr)
    , m_openglWindow(nullptr)
{
    setWindowTitle("QWindow OpenGL Demo");
    resize(800, 600);

    // 创建OpenGL窗口
    m_openglWindow = new OpenGLWindow();

    // 创建容器来承载OpenGL窗口
    m_container = QWidget::createWindowContainer(m_openglWindow, this);
    m_container->setMinimumSize(400, 300);

    setCentralWidget(m_container);
}

MainWindow::~MainWindow()
{
}