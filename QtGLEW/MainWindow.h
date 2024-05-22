
#pragma once
#include <QMainWindow>

class QAction;
class OpenGLWidget;

class  MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:

    void createActions();
    void createMenus();

private slots:
    void newFile();
private:
    QMenu *fileMenu;
    QAction *newAction;
    OpenGLWidget* canvas;
};
