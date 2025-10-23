#include <windows.h>
#include <gl/GL.h>

// 全局变量
HDC hDC;
HGLRC hRC;
HWND hWnd;
HINSTANCE hInstance;

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        // 渲染 OpenGL 场景
        wglMakeCurrent(hDC, hRC);
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

        SwapBuffers(hDC);
        ValidateRect(hWnd, NULL);
        break;

    case WM_DESTROY:
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hRC);
        ReleaseDC(hWnd, hDC);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 注册窗口类
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASS wc = {};
    wc.style = CS_OWNDC; // 必须，保证窗口有自己的DC
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("OpenGLWinClass");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    return RegisterClass(&wc);
}

// 创建 OpenGL 上下文
bool InitOpenGL(HWND hWnd)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,    // 颜色深度
        0,0,0,0,0,0,
        0,0,
        0,0,0,0,0,
        24,    // 深度缓冲
        8,     // 模板缓冲
        0,
        PFD_MAIN_PLANE,
        0,0,0,0
    };

    hDC = GetDC(hWnd);
    int pixelFormat = ChoosePixelFormat(hDC, &pfd);
    if (pixelFormat == 0) return false;

    if (!SetPixelFormat(hDC, pixelFormat, &pfd)) return false;

    hRC = wglCreateContext(hDC);
    if (!hRC) return false;

    if (!wglMakeCurrent(hDC, hRC)) return false;

    return true;
}

// WinMain 入口
int APIENTRY WinMain(HINSTANCE hInstance_, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    hInstance = hInstance_;
    MyRegisterClass(hInstance);

    hWnd = CreateWindow(TEXT("OpenGLWinClass"), TEXT("OpenGL Win32 Example"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 800, 600,
        NULL, NULL, hInstance, NULL);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    if (!InitOpenGL(hWnd))
        return -1;

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        InvalidateRect(hWnd, NULL, FALSE);
    }

    return (int)msg.wParam;
}
