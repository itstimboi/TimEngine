#include "Graphics.h"
#include "LegacyGL.h"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

bool isUsingGL1 = false;

static Renderer currentRenderer = Renderer::None;
static GLFWwindow* graphicsWindow = nullptr;


Renderer GetRenderer()
{
    return currentRenderer;
}


GLRenderer bestGLVersion()
{
    struct GLVersion
    {
        int major;
        int minor;
        GLRenderer renderer;
    };

    const GLVersion versions[] =
    {
        {4, 6, GLRenderer::gl46},
        {4, 5, GLRenderer::gl45},
        {4, 4, GLRenderer::gl44},
        {4, 3, GLRenderer::gl43},
        {4, 2, GLRenderer::gl42},
        {4, 1, GLRenderer::gl41},
        {4, 0, GLRenderer::gl40},

        {3, 3, GLRenderer::gl33},
        {3, 2, GLRenderer::gl32},
        {3, 1, GLRenderer::gl31},
        {3, 0, GLRenderer::gl30},

        {2, 1, GLRenderer::gl21},
        {2, 0, GLRenderer::gl20}
    };

    if (!glfwInit())
    {
        std::cout << "GLFW initialization failed.\n";

#ifdef _WIN32
        Renderer legacy = detectLegacyGL();

        if (legacy != Renderer::None)
        {
            isUsingGL1 = true;
            return legacy;
        }
#endif

        return GLRenderer::None;
    }

    for (const auto& version : versions)
    {
        glfwDefaultWindowHints();

        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MAJOR,
            version.major
        );

        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MINOR,
            version.minor
        );

        glfwWindowHint(
            GLFW_VISIBLE,
            GLFW_FALSE
        );

        std::string title =
            "OpenGL " +
            std::to_string(version.major) +
            "." +
            std::to_string(version.minor);

        GLFWwindow* window =
            glfwCreateWindow(
                100,
                100,
                title.c_str(),
                nullptr,
                nullptr
            );

        if (!window)
            continue;

        glfwMakeContextCurrent(window);

        const GLubyte* glVersion =
            glGetString(GL_VERSION);

        if (glVersion)
        {
            // std::cout
            //     << "Requested OpenGL "
            //     << version.major
            //     << "."
            //     << version.minor
            //     << '\n';

            // std::cout
            //     << "Actual OpenGL: "
            //     << glVersion
            //     << '\n';

            glfwDestroyWindow(window);
            glfwTerminate();

            isUsingGL1 = false;

            return version.renderer;
        }

        glfwDestroyWindow(window);
    }

    glfwTerminate();

#ifdef _WIN32
    Renderer legacy = detectLegacyGL();

    if (legacy != Renderer::None)
    {
        isUsingGL1 = true;
        return legacy;
    }
#endif

    return GLRenderer::None;
}


static const char* RendererName(Renderer renderer)
{
    if (renderer == Renderer::GL)
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
            default:
                return "None";
        }
    }
    else if (renderer == Renderer::DX)
    {
        switch (renderer)
        {
            case DXRenderer::dx120: return "DirectX 12.0";

            case DXRenderer::dx110: return "DirectX 11.0";

            case DXRenderer::dx100: return "DirectX 10.0";

            case DXRenderer::dx90c: return "DirectX 9.0c";
            case DXRenderer::dx90b: return "DirectX 9.0b";
            case DXRenderer::dx90a: return "DirectX 9.0a";
            case DXRenderer::dx90: return "DirectX 9.0";

            case DXRenderer::dx81b: return "DirectX 8.1b";
            case DXRenderer::dx81: return "DirectX 8.1";
            case DXRenderer::dx80: return "DirectX 8.0";

            case DXRenderer::dx70a: return "DirectX 7.0a";
            case DXRenderer::dx70: return "DirectX 7.0";
            
            case DXRenderer::dx61: return "DirectX 6.1";
            case DXRenderer::dx60: return "DirectX 6.0";
            
            case DXRenderer::dx52a: return "DirectX 5.2a";
            case DXRenderer::dx52: return "DirectX 5.2";
            case DXRenderer::dx51: return "DirectX 5.1";
            case DXRenderer::dx50: return "DirectX 5.0";

            case DXRenderer::dx30a: return "DirectX 3.0a";
            case DXRenderer::dx30: return "DirectX 3.0";

            case DXRenderer::dx20: return "DirectX 2.0";

            case DXRenderer::dx10: return "DirectX 1.0";

            case DXRenderer::None:
            default:
                return "None";
        }
    }
    else if (renderer == Renderer::VK)
    {
        switch (renderer)
        {
            case VKRenderer::vk14: return "Vulkan 1.4";

            case VKRenderer::vk13: return "Vulkan 1.3";

            case VKRenderer::None:
            default:
                return "None";
        }
    }
}


bool InitGraphics(
    int width,
    int height,
    const char* title
)
{
    currentRenderer = bestGLVersion();

    if (currentRenderer == Renderer::None)
    {
        std::cout
            << "Graphics initialization failed.\n";

        return false;
    }

    /*
        For now, GLFW3 is our actual rendering window.

        We request the version that the detector selected.
    */

    if (isUsingGL1)
    {
        std::cout
            << "Legacy OpenGL renderer selected.\n";

        /*
            The legacy WGL context currently belongs to
            detectLegacyGL() and has already been destroyed.

            We'll replace this with the permanent legacy
            window/context backend later.
        */

        return true;
    }

    if (!glfwInit())
    {
        std::cout
            << "Failed to initialize GLFW.\n";

        currentRenderer = Renderer::None;

        return false;
    }

    glfwDefaultWindowHints();

    int major = 0;
    int minor = 0;

    switch (currentRenderer)
    {
        case Renderer::gl46:
            major = 4;
            minor = 6;
            break;

        case Renderer::gl45:
            major = 4;
            minor = 5;
            break;

        case Renderer::gl44:
            major = 4;
            minor = 4;
            break;

        case Renderer::gl43:
            major = 4;
            minor = 3;
            break;

        case Renderer::gl42:
            major = 4;
            minor = 2;
            break;

        case Renderer::gl41:
            major = 4;
            minor = 1;
            break;

        case Renderer::gl40:
            major = 4;
            minor = 0;
            break;

        case Renderer::gl33:
            major = 3;
            minor = 3;
            break;

        case Renderer::gl32:
            major = 3;
            minor = 2;
            break;

        case Renderer::gl31:
            major = 3;
            minor = 1;
            break;

        case Renderer::gl30:
            major = 3;
            minor = 0;
            break;

        case Renderer::gl21:
            major = 2;
            minor = 1;
            break;

        case Renderer::gl20:
            major = 2;
            minor = 0;
            break;

        default:
            glfwTerminate();
            currentRenderer = Renderer::None;
            return false;
    }

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        major
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        minor
    );

    graphicsWindow = glfwCreateWindow(
        width,
        height,
        title,
        nullptr,
        nullptr
    );

    if (!graphicsWindow)
    {
        std::cout
            << "Failed to create graphics window.\n";

        glfwTerminate();

        currentRenderer = Renderer::None;

        return false;
    }

    glfwMakeContextCurrent(graphicsWindow);

    const GLubyte* actualVersion =
        glGetString(GL_VERSION);

    if (!actualVersion)
    {
        std::cout
            << "Failed to obtain OpenGL version.\n";

        glfwDestroyWindow(graphicsWindow);
        graphicsWindow = nullptr;

        glfwTerminate();

        currentRenderer = Renderer::None;

        return false;
    }

    std::cout
        << "Graphics initialized.\n";

    std::cout
        << "Renderer: "
        << RendererName(currentRenderer)
        << '\n';

    std::cout
        << "Actual OpenGL: "
        << actualVersion
        << '\n';

    return true;
}


void DeInitGraphics()
{
    if (graphicsWindow)
    {
        glfwDestroyWindow(graphicsWindow);
        graphicsWindow = nullptr;
    }

    glfwTerminate();

    currentRenderer = Renderer::None;
    isUsingGL1 = false;
}


void SwapGraphicsBuffers()
{
    if (graphicsWindow)
        glfwSwapBuffers(graphicsWindow);
}


bool GraphicsShouldClose()
{
    if (!graphicsWindow)
        return true;

    return glfwWindowShouldClose(graphicsWindow);
}


void PollGraphicsEvents()
{
    glfwPollEvents();
}