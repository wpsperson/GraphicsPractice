#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QtGui/QWindow>

QT_BEGIN_NAMESPACE
class QOpenGLContext;
QT_END_NAMESPACE

class OpenGLWindow : public QWindow
{
    Q_OBJECT

public:
    explicit OpenGLWindow(QWindow* parent = nullptr);
    ~OpenGLWindow();

    virtual void render();
    virtual void initialize();

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent* event) override;
    void exposeEvent(QExposeEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QOpenGLContext* m_context;
    bool m_update_pending;
};

#endif // OPENGLWINDOW_H