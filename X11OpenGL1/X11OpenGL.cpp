#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <iostream>
#include <cstdlib>

int main()
{
    // 打开 X server
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "无法打开 X Display\n";
        return EXIT_FAILURE;
    }

    // 获取默认屏幕
    int screen = DefaultScreen(display);

    // 配置属性
    static int visualAttribs[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_DEPTH_SIZE, 24,
        None
    };

    // 选择合适的 Visual
    XVisualInfo* vi = glXChooseVisual(display, screen, visualAttribs);
    if (!vi) {
        std::cerr << "没有找到合适的视觉 (visual)\n";
        return EXIT_FAILURE;
    }

    // 创建 X11 窗口
    Window root = RootWindow(display, vi->screen);
    Colormap cmap = XCreateColormap(display, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;

    Window win = XCreateWindow(display, root,
                               0, 0, 800, 600, 0,
                               vi->depth, InputOutput,
                               vi->visual,
                               CWColormap | CWEventMask, &swa);

    XStoreName(display, win, "OpenGL X11 Example");
    XMapWindow(display, win);

    // 创建 OpenGL 上下文
    GLXContext glc = glXCreateContext(display, vi, NULL, GL_TRUE);
    glXMakeCurrent(display, win, glc);

    // 设置视口
    glViewport(0, 0, 800, 600);

    // 主循环
    bool running = true;
    while (running) {
        while (XPending(display)) {
            XEvent xev;
            XNextEvent(display, &xev);
            if (xev.type == Expose) {
                // 窗口重绘
                glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                glBegin(GL_TRIANGLES);
                glColor3f(1, 0, 0);
                glVertex2f(-0.5f, -0.5f);
                glColor3f(0, 1, 0);
                glVertex2f(0.5f, -0.5f);
                glColor3f(0, 0, 1);
                glVertex2f(0.0f, 0.5f);
                glEnd();

                glXSwapBuffers(display, win);
            } else if (xev.type == KeyPress) {
                running = false; // 按任意键退出
            } else if (xev.type == DestroyNotify) {
                running = false;
            }
        }
    }

    // 清理
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, glc);
    XDestroyWindow(display, win);
    XCloseDisplay(display);

    return 0;
}
