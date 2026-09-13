#include "glfw3stuff.h"

#include <cstdio>

#ifdef _WIN32

#include "windowsversion.h"

#endif

#include <GLFW/glfw3.h>

namespace TE
{

// ============================================================
// Backend
// ============================================================

GLFWBackend GetGLFWBackend()
{
#ifdef _WIN32

    WindowsVersion version =
        GetWindowsVersion();

    switch (version)
    {
        // ----------------------------------------------------
        // Modern Windows
        // ----------------------------------------------------

        case WindowsVersion::Windows11:
        case WindowsVersion::Windows10:
        case WindowsVersion::Windows81:
        case WindowsVersion::Windows8:

            return GLFWBackend::GLFW3;


        // ----------------------------------------------------
        // Legacy Windows
        // ----------------------------------------------------

        case WindowsVersion::Windows7:
        case WindowsVersion::WindowsVista:
        case WindowsVersion::WindowsXP:
        case WindowsVersion::Windows2000:
        case WindowsVersion::WindowsME:
        case WindowsVersion::Windows98:
        case WindowsVersion::Windows95:

            return GLFWBackend::GLFW2;


        default:

            return GLFWBackend::None;
    }

#else

    // For now use GLFW3 on non-Windows.
    return GLFWBackend::GLFW3;

#endif
}


// ============================================================
// GLFW 3
// ============================================================

#if defined(TE_USE_GLFW3)

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#endif


// ============================================================
// GLFW 2
// ============================================================

#if defined(TE_USE_GLFW2)

#include <GLFW/glfw2.h>

#endif


// ============================================================
// Initialize
// ============================================================

bool InitializeGLFWBackend()
{
    GLFWBackend backend =
        GetGLFWBackend();


#if defined(TE_USE_GLFW3)

    if (backend == GLFWBackend::GLFW3)
    {
        return glfwInit() == GLFW_TRUE;
    }

#endif


#if defined(TE_USE_GLFW2)

    if (backend == GLFWBackend::GLFW2)
    {
        return glfw2Init() == GL_TRUE;
    }

#endif


    return false;
}


// ============================================================
// Shutdown
// ============================================================

void ShutdownGLFWBackend()
{

#if defined(TE_USE_GLFW3)

    if (GetGLFWBackend() == GLFWBackend::GLFW3)
    {
        glfwTerminate();
        return;
    }

#endif


#if defined(TE_USE_GLFW2)

    if (GetGLFWBackend() == GLFWBackend::GLFW2)
    {
        glfwTerminate();
        return;
    }

#endif

}


// ============================================================
// Configure OpenGL version
// ============================================================

bool SetGLFWOpenGLVersion(
    int major,
    int minor
)
{

#if defined(TE_USE_GLFW3)

    if (GetGLFWBackend() == GLFWBackend::GLFW3)
    {
        glfwDefaultWindowHints();

        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MAJOR,
            major
        );

        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MINOR,
            minor
        );

        glfwWindowHint(
            GLFW_VISIBLE,
            GLFW_FALSE
        );

        return true;
    }

#endif


#if defined(TE_USE_GLFW2)

    if (GetGLFWBackend() == GLFWBackend::GLFW2)
    {
        glfwOpenWindowHint(
            GLFW_OPENGL_VERSION_MAJOR,
            major
        );

        glfwOpenWindowHint(
            GLFW_OPENGL_VERSION_MINOR,
            minor
        );

        return true;
    }

#endif


    return false;
}


// ============================================================
// Create Window
// ============================================================

void* CreateGLFWWindow(
    int width,
    int height,
    const char* title,
    int glMajor,
    int glMinor,
    bool visible
)
{

#if defined(TE_USE_GLFW3)

    if (GetGLFWBackend() == GLFWBackend::GLFW3)
    {
        glfwDefaultWindowHints();

        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MAJOR,
            glMajor
        );

        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MINOR,
            glMinor
        );

        glfwWindowHint(
            GLFW_VISIBLE,
            visible
                ? GLFW_TRUE
                : GLFW_FALSE
        );

        GLFWwindow* window =
            glfwCreateWindow(
                width,
                height,
                title,
                nullptr,
                nullptr
            );

        if (!window)
            return nullptr;

        glfwMakeContextCurrent(window);

        return window;
    }

#endif


#if defined(TE_USE_GLFW2)

    if (GetGLFWBackend() == GLFWBackend::GLFW2)
    {
        glfw2OpenWindowHint(
            GLFW_OPENGL_VERSION_MAJOR,
            glMajor
        );

        glfw2OpenWindowHint(
            GLFW_OPENGL_VERSION_MINOR,
            glMinor
        );

        glfw2OpenWindowHint(
            GLFW_WINDOW_NO_RESIZE,
            GL_FALSE
        );

        int flags =
            visible
                ? GLFW_WINDOW
                : GLFW_WINDOW;

        if (!glfw2OpenWindow(
            width,
            height,
            0,
            0,
            0,
            0,
            24,
            8,
            flags))
        {
            return nullptr;
        }

        glfwSetWindowTitle(title);

        return reinterpret_cast<void*>(1);
    }

#endif


    return nullptr;
}


// ============================================================
// Destroy
// ============================================================

void DestroyGLFWWindow(void* window)
{
    if (!window)
        return;


#if defined(TE_USE_GLFW3)

    if (GetGLFWBackend() == GLFWBackend::GLFW3)
    {
        glfwDestroyWindow(
            static_cast<GLFWwindow*>(window)
        );

        return;
    }

#endif


#if defined(TE_USE_GLFW2)

    if (GetGLFWBackend() == GLFWBackend::GLFW2)
    {
        glfw2CloseWindow();

        return;
    }

#endif
}


// ============================================================
// Make Current
// ============================================================

void MakeGLFWContextCurrent(void* window)
{
    if (!window)
        return;


#if defined(TE_USE_GLFW3)

    if (GetGLFWBackend() == GLFWBackend::GLFW3)
    {
        glfwMakeContextCurrent(
            static_cast<GLFWwindow*>(window)
        );

        return;
    }

#endif


    // GLFW2's window/context is global,
    // so there is nothing to do here.
}


// ============================================================
// Swap
// ============================================================

void SwapGLFWBuffers(void* window)
{
    if (!window)
        return;


#if defined(TE_USE_GLFW3)

    if (GetGLFWBackend() == GLFWBackend::GLFW3)
    {
        glfwSwapBuffers(
            static_cast<GLFWwindow*>(window)
        );

        return;
    }

#endif


#if defined(TE_USE_GLFW2)

    if (GetGLFWBackend() == GLFWBackend::GLFW2)
    {
        glfw2SwapBuffers();

        return;
    }

#endif
}


// ============================================================
// Should Close
// ============================================================

bool GLFWWindowShouldClose(void* window)
{
    if (!window)
        return true;


#if defined(TE_USE_GLFW3)

    if (GetGLFWBackend() == GLFWBackend::GLFW3)
    {
        return glfwWindowShouldClose(
            static_cast<GLFWwindow*>(window)
        ) != 0;
    }

#endif


#if defined(TE_USE_GLFW2)

    if (GetGLFWBackend() == GLFWBackend::GLFW2)
    {
        return glfwGetWindowParam(
            GLFW_OPENED
        ) == GL_FALSE;
    }

#endif


    return true;
}


// ============================================================
// Events
// ============================================================

void PollGLFWEvents()
{

#if defined(TE_USE_GLFW3)

    if (GetGLFWBackend() == GLFWBackend::GLFW3)
    {
        glfwPollEvents();
        return;
    }

#endif


#if defined(TE_USE_GLFW2)

    if (GetGLFWBackend() == GLFWBackend::GLFW2)
    {
        // GLFW2 processes events through its event system.
        // glfwPollEvents() does not exist in the same form.
        // glfwPollEvents();
        return;
    }

#endif
}


// ============================================================
// GL version
// ============================================================

const char* GetGLVersionString()
{
    const GLubyte* version =
        glGetString(GL_VERSION);

    return reinterpret_cast<const char*>(
        version
    );
}

}