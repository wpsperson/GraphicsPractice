/*!  
 *@brief    
 *@author   wangps
 *@date     2022-8-7
 */

#pragma once
#include <QtWidgets/QOpenGLWidget>

class CanvasWidget : public QOpenGLWidget
{
public:
    CanvasWidget();
    ~CanvasWidget();
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
private:
    int m_width;
    int m_height;
    float m_x0 = 0.0f;
    float m_y0 = 0.0f;
    float m_x1 = 0.0f;
    float m_y1 = 0.0f;
    int  m_picked = 0;
};
