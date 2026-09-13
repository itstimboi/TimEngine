#include "Graphics.h"

#include "glfw3stuff.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

#ifdef _WIN32

    #include <windows.h>

    #include "windowsversion.h"

#endif

namespace TE
{

	// ============================================================
	// Internal State
	// ============================================================

	static std::vector<GraphicsWindow*> graphicsWindows;

	static GraphicsWindow* currentWindow = nullptr;

	static bool glfwInitialized = false;


	// ============================================================
	// GLFW Initialization
	// ============================================================

	GLFWBackend ConfigureGLFW()
	{

		TE::WindowsVersion windows = TE::GetWindowsVersion();

		switch (windows)
		{
			case TE::WindowsVersion::Windows11:
			case TE::WindowsVersion::Windows10:
			case TE::WindowsVersion::Windows81:
			case TE::WindowsVersion::Windows8:
			{
				// Modern GLFW
				printf("Modern GLFW configuration\n");

				
				
				break;
			}

			case TE::WindowsVersion::Windows7:
			case TE::WindowsVersion::WindowsVista:
			case TE::WindowsVersion::WindowsXP:
			case TE::WindowsVersion::Windows2000:
			case TE::WindowsVersion::WindowsME:
			case TE::WindowsVersion::Windows98:
			case TE::WindowsVersion::Windows95:
			{
				// Older GLFW configuration
				printf("Legacy GLFW configuration\n");
				break;
			}

			default:
			{
				printf("Unsupported Windows version\n");
				break;
			}
		}

	}

static bool InitializeGLFW()
{
    if (glfwInitialized)
        return true;

    glfwInitialized = ;

    return true;
}

// ============================================================
// Graphics Window
// ============================================================

struct GraphicsWindow
{
    GLFWwindow* handle = nullptr;

    int width = 0;
    int height = 0;

    std::string title;

    GraphicsRenderer renderer;

    // Only used for OpenGL 1.x/WGL contexts.
    bool legacyOpenGL = false;

#ifdef _WIN32
    HWND hwnd = nullptr;
    HDC hdc = nullptr;
    HGLRC hglrc = nullptr;
#endif
};

// static bool InitializeGLFW3()
// {
//     if (glfwInitialized)
//         return true;

//     if (!glfwInit())
//     {
//         std::cout
//             << "Failed to initialize GLFW.\n";

//         return false;
//     }

//     glfwInitialized = true;

//     return true;
// }

// ============================================================
// Renderer Names
// ============================================================

const char* RendererName(Renderer renderer)
{
    switch (renderer)
    {
        case Renderer::GL:
            return "OpenGL";

        case Renderer::DX:
            return "DirectX";

        case Renderer::VK:
            return "Vulkan";

        case Renderer::Software:
            return "Software";

        case Renderer::None:
            return "None";
    }

    return "Unknown";
}


const char* GLRendererName(GLRenderer renderer)
{
    switch (renderer)
    {
        case GLRenderer::gl46: return "OpenGL 4.6";
        case GLRenderer::gl45: return "OpenGL 4.5";
        case GLRenderer::gl44: return "OpenGL 4.4";
        case GLRenderer::gl43: return "OpenGL 4.3";
        case GLRenderer::gl42: return "OpenGL 4.2";
        case GLRenderer::gl41: return "OpenGL 4.1";
        case GLRenderer::gl40: return "OpenGL 4.0";

        case GLRenderer::gl33: return "OpenGL 3.3";
        case GLRenderer::gl32: return "OpenGL 3.2";
        case GLRenderer::gl31: return "OpenGL 3.1";
        case GLRenderer::gl30: return "OpenGL 3.0";

        case GLRenderer::gl21: return "OpenGL 2.1";
        case GLRenderer::gl20: return "OpenGL 2.0";

        case GLRenderer::gl15: return "OpenGL 1.5";
        case GLRenderer::gl14: return "OpenGL 1.4";
        case GLRenderer::gl13: return "OpenGL 1.3";
        case GLRenderer::gl12: return "OpenGL 1.2";
        case GLRenderer::gl11: return "OpenGL 1.1";
        case GLRenderer::gl10: return "OpenGL 1.0";

        case GLRenderer::None:
            return "None";
    }

    return "Unknown";
}


const char* DXRendererName(DXRenderer renderer)
{
    switch (renderer)
    {
        case DXRenderer::dx120: return "DirectX 12";
        case DXRenderer::dx110: return "DirectX 11";
        case DXRenderer::dx100: return "DirectX 10";

        case DXRenderer::dx90c: return "DirectX 9.0c";
        case DXRenderer::dx90b: return "DirectX 9.0b";
        case DXRenderer::dx90a: return "DirectX 9.0a";
        case DXRenderer::dx90:  return "DirectX 9";

        case DXRenderer::dx81b: return "DirectX 8.1b";
        case DXRenderer::dx81:  return "DirectX 8.1";
        case DXRenderer::dx80:  return "DirectX 8.0";

        case DXRenderer::dx70a: return "DirectX 7.0a";
        case DXRenderer::dx70:  return "DirectX 7.0";

        case DXRenderer::dx61: return "DirectX 6.1";
        case DXRenderer::dx60: return "DirectX 6.0";

        case DXRenderer::dx52a: return "DirectX 5.2a";
        case DXRenderer::dx52:  return "DirectX 5.2";
        case DXRenderer::dx51:  return "DirectX 5.1";
        case DXRenderer::dx50:  return "DirectX 5.0";

        case DXRenderer::dx30a: return "DirectX 3.0a";
        case DXRenderer::dx30:  return "DirectX 3.0";

        case DXRenderer::dx20: return "DirectX 2.0";
        case DXRenderer::dx10: return "DirectX 1.0";

        case DXRenderer::None:
            return "None";
    }

    return "Unknown";
}


const char* VKRendererName(VKRenderer renderer)
{
    switch (renderer)
    {
        case VKRenderer::vk14:
            return "Vulkan 1.4";

        case VKRenderer::vk13:
            return "Vulkan 1.3";

        case VKRenderer::None:
            return "None";
    }

    return "Unknown";
}


// ============================================================
// OpenGL Version Conversion
// ============================================================

static void GetGLVersionNumbers(
    GLRenderer renderer,
    int& major,
    int& minor
)
{
    switch (renderer)
    {
        case GLRenderer::gl46:
            major = 4; minor = 6;
            break;

        case GLRenderer::gl45:
            major = 4; minor = 5;
            break;

        case GLRenderer::gl44:
            major = 4; minor = 4;
            break;

        case GLRenderer::gl43:
            major = 4; minor = 3;
            break;

        case GLRenderer::gl42:
            major = 4; minor = 2;
            break;

        case GLRenderer::gl41:
            major = 4; minor = 1;
            break;

        case GLRenderer::gl40:
            major = 4; minor = 0;
            break;

        case GLRenderer::gl33:
            major = 3; minor = 3;
            break;

        case GLRenderer::gl32:
            major = 3; minor = 2;
            break;

        case GLRenderer::gl31:
            major = 3; minor = 1;
            break;

        case GLRenderer::gl30:
            major = 3; minor = 0;
            break;

        case GLRenderer::gl21:
            major = 2; minor = 1;
            break;

        case GLRenderer::gl20:
            major = 2; minor = 0;
            break;

        default:
            major = 0;
            minor = 0;
            break;
    }
}


// ============================================================
// Legacy OpenGL Detection
// ============================================================

static GLRenderer DetectLegacyOpenGL()
{
#ifdef _WIN32

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
        {
            return GLRenderer::None;
        }
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
        return GLRenderer::None;

    HDC dc = GetDC(hwnd);

    if (!dc)
    {
        DestroyWindow(hwnd);
        return GLRenderer::None;
    }

    PIXELFORMATDESCRIPTOR pfd = {};

    pfd.nSize =
        sizeof(PIXELFORMATDESCRIPTOR);

    pfd.nVersion = 1;

    pfd.dwFlags =
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;

    pfd.iPixelType =
        PFD_TYPE_RGBA;

    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixelFormat =
        ChoosePixelFormat(
            dc,
            &pfd
        );

    if (pixelFormat == 0)
    {
        ReleaseDC(hwnd, dc);
        DestroyWindow(hwnd);

        return GLRenderer::None;
    }

    if (!SetPixelFormat(
        dc,
        pixelFormat,
        &pfd))
    {
        ReleaseDC(hwnd, dc);
        DestroyWindow(hwnd);

        return GLRenderer::None;
    }

    HGLRC context =
        wglCreateContext(dc);

    if (!context)
    {
        ReleaseDC(hwnd, dc);
        DestroyWindow(hwnd);

        return GLRenderer::None;
    }

    if (!wglMakeCurrent(
        dc,
        context))
    {
        wglDeleteContext(context);

        ReleaseDC(hwnd, dc);
        DestroyWindow(hwnd);

        return GLRenderer::None;
    }

    const GLubyte* versionString =
        glGetString(GL_VERSION);

    if (!versionString)
    {
        wglMakeCurrent(
            nullptr,
            nullptr
        );

        wglDeleteContext(context);

        ReleaseDC(hwnd, dc);
        DestroyWindow(hwnd);

        return GLRenderer::None;
    }

    std::cout
        << "Legacy OpenGL: "
        << reinterpret_cast<const char*>(
            versionString
        )
        << '\n';

    int major = 1;
    int minor = 1;

    if (sscanf(
        reinterpret_cast<const char*>(
            versionString
        ),
        "%d.%d",
        &major,
        &minor
    ) != 2)
    {
        major = 1;
        minor = 1;
    }

    GLRenderer renderer =
        GLRenderer::gl11;

    if (major > 1 ||
        (major == 1 && minor >= 5))
    {
        renderer = GLRenderer::gl15;
    }
    else if (major == 1 && minor >= 4)
    {
        renderer = GLRenderer::gl14;
    }
    else if (major == 1 && minor >= 3)
    {
        renderer = GLRenderer::gl13;
    }
    else if (major == 1 && minor >= 2)
    {
        renderer = GLRenderer::gl12;
    }
    else
    {
        renderer = GLRenderer::gl11;
    }

    wglMakeCurrent(
        nullptr,
        nullptr
    );

    wglDeleteContext(context);

    ReleaseDC(hwnd, dc);
    DestroyWindow(hwnd);

    return renderer;

#else

    return GLRenderer::None;

#endif
}


// ============================================================
// Best OpenGL Version
// ============================================================

GLRenderer bestGLVersion()
{
    if (!InitializeGLFW())
        return GLRenderer::None;

    const GLRenderer versions[] =
    {
        GLRenderer::gl46,
        GLRenderer::gl45,
        GLRenderer::gl44,
        GLRenderer::gl43,
        GLRenderer::gl42,
        GLRenderer::gl41,
        GLRenderer::gl40,

        GLRenderer::gl33,
        GLRenderer::gl32,
        GLRenderer::gl31,
        GLRenderer::gl30,

        GLRenderer::gl21,
        GLRenderer::gl20
    };

    for (GLRenderer version : versions)
    {
        int major;
        int minor;

        GetGLVersionNumbers(
            version,
            major,
            minor
        );

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

        GLFWwindow* testWindow =
            glfwCreateWindow(
                1,
                1,
                "TimEngine GL Test",
                nullptr,
                nullptr
            );

        if (!testWindow)
            continue;

        glfwMakeContextCurrent(
            testWindow
        );

        const char* actualVersion =
            reinterpret_cast<const char*>(
                glGetString(GL_VERSION)
            );

        if (actualVersion)
        {
            std::cout
                << "Requested OpenGL "
                << major
                << '.'
                << minor
                << '\n';

            std::cout
                << "Actual OpenGL: "
                << actualVersion
                << '\n';
        }

        glfwDestroyWindow(
            testWindow
        );

        return version;
    }

    // GLFW doesn't give us a useful way to
    // request OpenGL 1.x, so use WGL.
    return DetectLegacyOpenGL();
}


// ============================================================
// OpenGL Window Creation
// ============================================================

static bool InitOpenGLWindow(
    GraphicsWindow* window,
    GLRenderer renderer
)
{
    if (!window)
        return false;

    // --------------------------------------------------------
    // Legacy OpenGL 1.x
    // --------------------------------------------------------

    if (renderer == GLRenderer::gl15 ||
        renderer == GLRenderer::gl14 ||
        renderer == GLRenderer::gl13 ||
        renderer == GLRenderer::gl12 ||
        renderer == GLRenderer::gl11 ||
        renderer == GLRenderer::gl10)
    {
#ifdef _WIN32

        HINSTANCE instance =
            GetModuleHandleA(nullptr);

        WNDCLASSA wc = {};

        wc.style = CS_OWNDC;
        wc.lpfnWndProc = DefWindowProcA;
        wc.hInstance = instance;
        wc.lpszClassName =
            "TimEngineLegacyGLWindow";

        if (!RegisterClassA(&wc))
        {
            if (GetLastError() !=
                ERROR_CLASS_ALREADY_EXISTS)
            {
                return false;
            }
        }

        HWND hwnd = CreateWindowA(
            "TimEngineLegacyGLWindow",
            window->title.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            window->width,
            window->height,
            nullptr,
            nullptr,
            instance,
            nullptr
        );

        if (!hwnd)
            return false;

        HDC dc = GetDC(hwnd);

        if (!dc)
        {
            DestroyWindow(hwnd);
            return false;
        }

        PIXELFORMATDESCRIPTOR pfd = {};

        pfd.nSize =
            sizeof(PIXELFORMATDESCRIPTOR);

        pfd.nVersion = 1;

        pfd.dwFlags =
            PFD_DRAW_TO_WINDOW |
            PFD_SUPPORT_OPENGL |
            PFD_DOUBLEBUFFER;

        pfd.iPixelType =
            PFD_TYPE_RGBA;

        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;

        int pixelFormat =
            ChoosePixelFormat(
                dc,
                &pfd
            );

        if (!pixelFormat)
        {
            ReleaseDC(hwnd, dc);
            DestroyWindow(hwnd);

            return false;
        }

        if (!SetPixelFormat(
            dc,
            pixelFormat,
            &pfd))
        {
            ReleaseDC(hwnd, dc);
            DestroyWindow(hwnd);

            return false;
        }

        HGLRC context =
            wglCreateContext(dc);

        if (!context)
        {
            ReleaseDC(hwnd, dc);
            DestroyWindow(hwnd);

            return false;
        }

        if (!wglMakeCurrent(
            dc,
            context))
        {
            wglDeleteContext(context);

            ReleaseDC(hwnd, dc);
            DestroyWindow(hwnd);

            return false;
        }

        window->hwnd = hwnd;
        window->hdc = dc;
        window->hglrc = context;

        window->legacyOpenGL = true;

        window->renderer.API =
            Renderer::GL;

        window->renderer.GL =
            renderer;

        window->renderer.DX =
            DXRenderer::None;

        window->renderer.VK =
            VKRenderer::None;

        ShowWindow(
            hwnd,
            SW_SHOW
        );

        std::cout
            << "Created "
            << GLRendererName(renderer)
            << " window.\n";

        return true;

#else

        std::cout
            << "Legacy OpenGL is not implemented "
            << "on this platform yet.\n";

        return false;

#endif
    }


    // --------------------------------------------------------
    // Modern OpenGL
    // --------------------------------------------------------

    int major;
    int minor;

    GetGLVersionNumbers(
        renderer,
        major,
        minor
    );

    if (major == 0)
        return false;

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
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_ANY_PROFILE
    );

    GLFWwindow* handle =
        glfwCreateWindow(
            window->width,
            window->height,
            window->title.c_str(),
            nullptr,
            nullptr
        );

    if (!handle)
        return false;

    window->handle =
        handle;

    window->legacyOpenGL =
        false;

    window->renderer.API =
        Renderer::GL;

    window->renderer.GL =
        renderer;

    window->renderer.DX =
        DXRenderer::None;

    window->renderer.VK =
        VKRenderer::None;

    glfwMakeContextCurrent(
        handle
    );

    const char* actualVersion =
        reinterpret_cast<const char*>(
            glGetString(GL_VERSION)
        );

    std::cout
        << "Created "
        << GLRendererName(renderer)
        << '\n';

    if (actualVersion)
    {
        std::cout
            << "Actual OpenGL: "
            << actualVersion
            << '\n';
    }

    return true;
}


// ============================================================
// Renderer Detection
// ============================================================

GraphicsRenderer DetectBestRenderer()
{
    GraphicsRenderer renderer;

    // --------------------------------------------------------
    // Vulkan
    // --------------------------------------------------------

    // TODO:
    // Detect Vulkan 1.4 / 1.3 here.


    // --------------------------------------------------------
    // DirectX
    // --------------------------------------------------------

    // TODO:
    // Detect DirectX here.


    // --------------------------------------------------------
    // OpenGL
    // --------------------------------------------------------

    GLRenderer gl =
        bestGLVersion();

    if (gl != GLRenderer::None)
    {
        renderer.API =
            Renderer::GL;

        renderer.GL =
            gl;

        renderer.DX =
            DXRenderer::None;

        renderer.VK =
            VKRenderer::None;

        return renderer;
    }


    // --------------------------------------------------------
    // Software
    // --------------------------------------------------------

    renderer.API =
        Renderer::Software;

    renderer.GL =
        GLRenderer::None;

    renderer.DX =
        DXRenderer::None;

    renderer.VK =
        VKRenderer::None;

    return renderer;
}


// ============================================================
// Create Graphics Window
// ============================================================

GraphicsWindow* CreateGraphicsWindow(
    int width,
    int height,
    const char* title
)
{
    GraphicsRenderer renderer =
        DetectBestRenderer();

    return CreateGraphicsWindow(
        width,
        height,
        title,
        renderer
    );
}


GraphicsWindow* CreateGraphicsWindow(
    int width,
    int height,
    const char* title,
    GraphicsRenderer renderer
)
{
    if (!InitializeGLFW())
        return nullptr;

    if (!title)
        title = "TimEngine";

    GraphicsWindow* window =
        new GraphicsWindow();

    window->width =
        width;

    window->height =
        height;

    window->title =
        title;

    window->renderer =
        GraphicsRenderer{};

    window->legacyOpenGL =
        false;


    // --------------------------------------------------------
    // OpenGL
    // --------------------------------------------------------

    if (renderer.API == Renderer::GL)
    {
        if (renderer.GL ==
            GLRenderer::None)
        {
            delete window;
            return nullptr;
        }

        if (!InitOpenGLWindow(
            window,
            renderer.GL))
        {
            delete window;
            return nullptr;
        }
    }


    // --------------------------------------------------------
    // DirectX
    // --------------------------------------------------------

    else if (renderer.API == Renderer::DX)
    {
        std::cout
            << "DirectX renderer is not implemented yet.\n";

        delete window;
        return nullptr;
    }


    // --------------------------------------------------------
    // Vulkan
    // --------------------------------------------------------

    else if (renderer.API == Renderer::VK)
    {
        std::cout
            << "Vulkan renderer is not implemented yet.\n";

        delete window;
        return nullptr;
    }


    // --------------------------------------------------------
    // Software
    // --------------------------------------------------------

    else if (renderer.API == Renderer::Software)
    {
        std::cout
            << "Software renderer is not implemented yet.\n";

        delete window;
        return nullptr;
    }


    else
    {
        delete window;
        return nullptr;
    }


    graphicsWindows.push_back(
        window
    );

    currentWindow =
        window;

    return window;
}


// ============================================================
// Destroy Graphics Window
// ============================================================

void DestroyGraphicsWindow(
    GraphicsWindow* window
)
{
    if (!window)
        return;


    // --------------------------------------------------------
    // Legacy OpenGL
    // --------------------------------------------------------

    if (window->legacyOpenGL)
    {
#ifdef _WIN32

        if (window->hglrc)
        {
            if (wglGetCurrentContext() ==
                window->hglrc)
            {
                wglMakeCurrent(
                    nullptr,
                    nullptr
                );
            }

            wglDeleteContext(
                window->hglrc
            );

            window->hglrc =
                nullptr;
        }

        if (window->hwnd &&
            window->hdc)
        {
            ReleaseDC(
                window->hwnd,
                window->hdc
            );
        }

        if (window->hwnd)
        {
            DestroyWindow(
                window->hwnd
            );
        }

        window->hdc =
            nullptr;

        window->hwnd =
            nullptr;

#endif
    }


    // --------------------------------------------------------
    // GLFW
    // --------------------------------------------------------

    if (window->handle)
    {
        if (currentWindow == window)
        {
            glfwMakeContextCurrent(
                nullptr
            );
        }

        glfwDestroyWindow(
            window->handle
        );

        window->handle =
            nullptr;
    }


    // Remove from list.

    for (auto it =
        graphicsWindows.begin();
        it != graphicsWindows.end();
        ++it)
    {
        if (*it == window)
        {
            graphicsWindows.erase(it);
            break;
        }
    }


    if (currentWindow == window)
    {
        currentWindow =
            graphicsWindows.empty()
                ? nullptr
                : graphicsWindows.front();
    }

    delete window;
}


// ============================================================
// Switch Renderer
// ============================================================

bool SwitchRenderer(
    GraphicsWindow* window,
    GraphicsRenderer renderer
)
{
    if (!window)
        return false;

    int width =
        window->width;

    int height =
        window->height;

    std::string title =
        window->title;


    // Destroy current graphics context.

    DestroyGraphicsWindow(
        window
    );


    // IMPORTANT:
    //
    // The pointer passed into this function
    // was destroyed above.
    //
    // We therefore cannot recreate it in-place
    // with the current API.
    //
    // This will be redesigned when we add the
    // renderer/resource backend system.

    (void)width;
    (void)height;
    (void)title;
    (void)renderer;

    return false;
}


// ============================================================
// Make Current
// ============================================================

void GraphicsMakeCurrent(
    GraphicsWindow* window
)
{
    if (!window)
        return;

    currentWindow =
        window;

    if (window->legacyOpenGL)
    {
#ifdef _WIN32

        wglMakeCurrent(
            window->hdc,
            window->hglrc
        );

#endif
    }
    else if (window->handle)
    {
        glfwMakeContextCurrent(
            window->handle
        );
    }
}


// ============================================================
// Window Information
// ============================================================

int GraphicsGetWidth(
    GraphicsWindow* window
)
{
    if (!window)
        return 0;

    return window->width;
}


int GraphicsGetHeight(
    GraphicsWindow* window
)
{
    if (!window)
        return 0;

    return window->height;
}


// ============================================================
// Renderer Information
// ============================================================

GraphicsRenderer GetGraphicsRenderer(
    GraphicsWindow* window
)
{
    if (!window)
        return GraphicsRenderer{};

    return window->renderer;
}


Renderer GetRenderer(
    GraphicsWindow* window
)
{
    if (!window)
        return Renderer::None;

    return window->renderer.API;
}


GLRenderer GetGLRenderer(
    GraphicsWindow* window
)
{
    if (!window)
        return GLRenderer::None;

    return window->renderer.GL;
}


DXRenderer GetDXRenderer(
    GraphicsWindow* window
)
{
    if (!window)
        return DXRenderer::None;

    return window->renderer.DX;
}


VKRenderer GetVKRenderer(
    GraphicsWindow* window
)
{
    if (!window)
        return VKRenderer::None;

    return window->renderer.VK;
}


// ============================================================
// Legacy OpenGL State
// ============================================================

bool IsUsingLegacyOpenGL(
    GraphicsWindow* window
)
{
    if (!window)
        return false;

    return window->legacyOpenGL;
}


// ============================================================
// Swap Buffers
// ============================================================

void GraphicsSwapBuffers(
    GraphicsWindow* window
)
{
    if (!window)
        return;


    if (window->renderer.API ==
        Renderer::GL)
    {
        if (window->legacyOpenGL)
        {
#ifdef _WIN32

            SwapBuffers(
                window->hdc
            );

#endif
        }
        else if (window->handle)
        {
            glfwSwapBuffers(
                window->handle
            );
        }
    }


    // TODO:
    // DirectX Present()
    // Vulkan vkQueuePresentKHR()
    // Software framebuffer presentation
}


// ============================================================
// Window Close
// ============================================================

bool GraphicsWindowShouldClose(
    GraphicsWindow* window
)
{
    if (!window)
        return true;

    if (window->legacyOpenGL)
    {
#ifdef _WIN32

        return !IsWindow(
            window->hwnd
        );

#else

        return false;

#endif
    }

    if (window->handle)
    {
        return glfwWindowShouldClose(
            window->handle
        );
    }

    return true;
}


// ============================================================
// Event Processing
// ============================================================

void GraphicsPollEvents()
{
    if (!glfwInitialized)
        return;

    glfwPollEvents();
}


// ============================================================
// Shutdown
// ============================================================

void DeInitGraphics()
{
    while (!graphicsWindows.empty())
    {
        DestroyGraphicsWindow(
            graphicsWindows.back()
        );
    }

    currentWindow =
        nullptr;

    if (glfwInitialized)
    {
        glfwTerminate();

        glfwInitialized =
            false;
    }
}

}