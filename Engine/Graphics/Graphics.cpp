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


Renderer bestGLVersion()
{
    struct GLVersion
    {
        int major;
        int minor;
        Renderer renderer;
    };

    const GLVersion versions[] =
    {
        {4, 6, Renderer::gl46},
        {4, 5, Renderer::gl45},
        {4, 4, Renderer::gl44},
        {4, 3, Renderer::gl43},
        {4, 2, Renderer::gl42},
        {4, 1, Renderer::gl41},
        {4, 0, Renderer::gl40},

        {3, 3, Renderer::gl33},
        {3, 2, Renderer::gl32},
        {3, 1, Renderer::gl31},
        {3, 0, Renderer::gl30},

        {2, 1, Renderer::gl21},
        {2, 0, Renderer::gl20}
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

        return Renderer::None;
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

    return Renderer::None;
}


static const char* RendererName(Renderer renderer)
{
    switch (renderer)
    {
        case Renderer::gl46: return "OpenGL 4.6";
        case Renderer::gl45: return "OpenGL 4.5";
        case Renderer::gl44: return "OpenGL 4.4";
        case Renderer::gl43: return "OpenGL 4.3";
        case Renderer::gl42: return "OpenGL 4.2";
        case Renderer::gl41: return "OpenGL 4.1";
        case Renderer::gl40: return "OpenGL 4.0";

        case Renderer::gl33: return "OpenGL 3.3";
        case Renderer::gl32: return "OpenGL 3.2";
        case Renderer::gl31: return "OpenGL 3.1";
        case Renderer::gl30: return "OpenGL 3.0";

        case Renderer::gl21: return "OpenGL 2.1";
        case Renderer::gl20: return "OpenGL 2.0";

        case Renderer::gl15: return "OpenGL 1.5";
        case Renderer::gl14: return "OpenGL 1.4";
        case Renderer::gl13: return "OpenGL 1.3";
        case Renderer::gl12: return "OpenGL 1.2";
        case Renderer::gl11: return "OpenGL 1.1";
        case Renderer::gl10: return "OpenGL 1.0";

        case Renderer::None:
        default:
            return "None";
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