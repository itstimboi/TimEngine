#include "Graphics.h"

#include <GL/gl.h>

#ifdef _WIN32

    #if _WIN32_WINNT <= 0x0601
        #include "GLFW/glfw2.h"
    #endif
    
#else

    #include "GLFW/glfw3.h"

#endif

#include <string>

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
        {2, 0, Renderer::gl20},

        {1, 5, Renderer::gl15},
        {1, 4, Renderer::gl14},
        {1, 3, Renderer::gl13},
        {1, 2, Renderer::gl12},
        {1, 1, Renderer::gl11},
        {1, 0, Renderer::gl10}
    };

    for (const auto& version : versions)
    {
        glfwDefaultWindowHints();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.minor);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        std::string title =
            "GL Version Test for: OpenGL " +
            std::to_string(version.major) + "." +
            std::to_string(version.minor);

        GLFWwindow* window = glfwCreateWindow(
            100,
            100,
            title.c_str(),
            nullptr,
            nullptr
        );

        if (window)
        {
            glfwMakeContextCurrent(window);

            const GLubyte* glVersion =
                glGetString(GL_VERSION);

            if (glVersion)
            {
                if (version.major == 1)
                    isUsingGL1 = true;
                else
                    isUsingGL1 = false;

                glfwDestroyWindow(window);
                return version.renderer;
            }

            glfwDestroyWindow(window);

        }
    }

    isUsingGL1 = true;

    // Nothing worked
    return Renderer::gl10;
}