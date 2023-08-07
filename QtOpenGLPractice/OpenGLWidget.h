#pragma once
#include <QtWidgets/QOpenGLWidget>


class Renderer;
class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    //void contextMenuEvent(QContextMenuEvent* event) override;
private:
    int m_width = 0;
    int m_height = 0;
    Renderer* m_renderer = nullptr;
};

