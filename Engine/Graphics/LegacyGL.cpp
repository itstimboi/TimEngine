#include "LegacyGL.h"

#ifdef _WIN32

    #include <windows.h>
    #include <GL/gl.h>
    #include <iostream>

    Renderer detectLegacyGL()
    {
        HINSTANCE instance =
            GetModuleHandleA(nullptr);

        WNDCLASSA wc = {};

        wc.style = CS_OWNDC;
        wc.lpfnWndProc = DefWindowProcA;
        wc.hInstance = instance;
        wc.lpszClassName = "TimEngineLegacyGL";

        if (!RegisterClassA(&wc))
        {
            if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
                return Renderer::None;
        }

        HWND hwnd = CreateWindowA(
            "TimEngineLegacyGL",
            "TimEngine Legacy GL Test",
            WS_POPUP,
            0,
            0,
            1,
            1,
            nullptr,
            nullptr,
            instance,
            nullptr
        );

        if (!hwnd)
            return Renderer::None;

        HDC dc = GetDC(hwnd);

        PIXELFORMATDESCRIPTOR pfd = {};

        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;

        pfd.dwFlags =
            PFD_DRAW_TO_WINDOW |
            PFD_SUPPORT_OPENGL |
            PFD_DOUBLEBUFFER;

        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;

        int pixelFormat = ChoosePixelFormat(
            dc,
            &pfd
        );

        if (pixelFormat == 0)
        {
            ReleaseDC(hwnd, dc);
            DestroyWindow(hwnd);
            return Renderer::None;
        }

        if (!SetPixelFormat(
            dc,
            pixelFormat,
            &pfd))
        {
            ReleaseDC(hwnd, dc);
            DestroyWindow(hwnd);
            return Renderer::None;
        }

        HGLRC context = wglCreateContext(dc);

        if (!context)
        {
            ReleaseDC(hwnd, dc);
            DestroyWindow(hwnd);
            return Renderer::None;
        }

        if (!wglMakeCurrent(dc, context))
        {
            wglDeleteContext(context);
            ReleaseDC(hwnd, dc);
            DestroyWindow(hwnd);
            return Renderer::None;
        }

        const GLubyte* versionString =
            glGetString(GL_VERSION);

        if (!versionString)
        {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(context);
            ReleaseDC(hwnd, dc);
            DestroyWindow(hwnd);
            return Renderer::None;
        }

        std::cout
            << "Legacy OpenGL: "
            << versionString
            << '\n';

        int major = 1;
        int minor = 1;

        if (sscanf(
            reinterpret_cast<const char*>(versionString),
            "%d.%d",
            &major,
            &minor) != 2)
        {
            major = 1;
            minor = 1;
        }

        Renderer renderer = Renderer::gl11;

        if (major > 1 || (major == 1 && minor >= 5))
            renderer = Renderer::gl15;
        else if (major == 1 && minor >= 4)
            renderer = Renderer::gl14;
        else if (major == 1 && minor >= 3)
            renderer = Renderer::gl13;
        else if (major == 1 && minor >= 2)
            renderer = Renderer::gl12;
        else
            renderer = Renderer::gl11;

        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(context);

        ReleaseDC(hwnd, dc);
        DestroyWindow(hwnd);

        return renderer;
    }

#endif