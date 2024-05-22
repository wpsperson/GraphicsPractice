#pragma once
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QProgressDialog>


class Renderer;
class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();
    void setPopup(bool popup);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
private:
    int m_width = 0;
    int m_height = 0;
    Renderer* m_renderer = nullptr;
    bool m_popup = false;
};

