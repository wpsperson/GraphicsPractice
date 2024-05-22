#pragma once

#include <QMainWindow>

class OpenGLWidget;

class  DummyWidget : public QMainWindow
{
    Q_OBJECT
public:
    DummyWidget(QWidget* parent = nullptr);
    ~DummyWidget();

private:
    OpenGLWidget* canvas;
};
